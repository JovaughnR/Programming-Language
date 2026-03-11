"use client";

import { useState, useCallback } from "react";
import dynamic from "next/dynamic";
import Sidebar from "./components/Sidebar";
import TabBar from "./components/TabBar";
import OutputPanel from "./components/OutputPanel";
import StatusBar from "./components/StatusBar";
import SnippetsPanel from "./components/SnippetsPanel";
import Toolbar from "./components/Toolbar";

// Monaco must be loaded client-side only
const CodeEditor = dynamic(() => import("./components/CodeEditor"), {
	ssr: false,
});

export type Tab = {
	id: string;
	name: string;
	code: string;
	isModified: boolean;
};

export type OutputLine = {
	id: string;
	type: "stdout" | "stderr" | "info" | "system";
	text: string;
	timestamp: string;
};

export type DiagnosticMarker = {
	startLineNumber: number;
	startColumn: number;
	endLineNumber: number;
	endColumn: number;
	message: string;
	severity: "error" | "warning";
};

const DEFAULT_TABS: Tab[] = [
	{
		id: "1",
		name: "main.cx",
		isModified: false,
		code: `# Welcome to cxIDE
# Write your custom language code here

func greet(name) {
  print("Hello, " + name + "!");
}

greet("World");
`,
	},
	{
		id: "2",
		name: "examples.cx",
		isModified: false,
		code: `# Fibonacci example

func fib(n) {
	if (n <= 1) {
		return n;
	}
	return fib(n - 1) + fib(n - 2);
}

i = 0;
while (i < 10) {
	print(fib(i));
	i = i + 1;
}
`,
	},
];

// ── Stdin Modal ───────────────────────────────────────────────────────────────
function StdinModal({
	inputCount,
	onSubmit,
	onCancel,
}: {
	inputCount: number;
	onSubmit: (stdin: string) => void;
	onCancel: () => void;
}) {
	const [value, setValue] = useState("");

	return (
		<div
			style={{
				position: "fixed",
				inset: 0,
				background: "rgba(0,0,0,0.6)",
				display: "flex",
				alignItems: "center",
				justifyContent: "center",
				zIndex: 1000,
			}}
		>
			<div
				style={{
					background: "var(--bg-panel, #1e1e1e)",
					border: "1px solid var(--border, #333)",
					borderRadius: "8px",
					padding: "24px",
					width: "420px",
					display: "flex",
					flexDirection: "column",
					gap: "12px",
					boxShadow: "0 8px 32px rgba(0,0,0,0.5)",
				}}
			>
				<div
					style={{
						color: "var(--text-primary, #d4d4d4)",
						fontFamily: "JetBrains Mono, monospace",
						fontSize: "13px",
						fontWeight: 600,
					}}
				>
					Program requires input
				</div>
				<div
					style={{
						color: "var(--text-muted, #858585)",
						fontFamily: "JetBrains Mono, monospace",
						fontSize: "12px",
					}}
				>
					{inputCount} input() call{inputCount > 1 ? "s" : ""} detected. Enter one
					value per line:
				</div>
				<textarea
					autoFocus
					value={value}
					onChange={(e) => setValue(e.target.value)}
					placeholder={`line 1\nline 2\n...`}
					rows={Math.min(inputCount + 1, 8)}
					style={{
						background: "var(--bg-primary, #141414)",
						color: "var(--text-primary, #d4d4d4)",
						border: "1px solid var(--border, #333)",
						borderRadius: "4px",
						padding: "8px",
						fontFamily: "JetBrains Mono, monospace",
						fontSize: "13px",
						resize: "vertical",
						outline: "none",
					}}
				/>
				<div style={{ display: "flex", gap: "8px", justifyContent: "flex-end" }}>
					<button
						onClick={onCancel}
						style={{
							padding: "6px 16px",
							borderRadius: "4px",
							border: "1px solid var(--border, #333)",
							background: "transparent",
							color: "var(--text-muted, #858585)",
							fontFamily: "JetBrains Mono, monospace",
							fontSize: "12px",
							cursor: "pointer",
						}}
					>
						Cancel
					</button>
					<button
						onClick={() => onSubmit(value)}
						style={{
							padding: "6px 16px",
							borderRadius: "4px",
							border: "none",
							background: "#0078d4",
							color: "#fff",
							fontFamily: "JetBrains Mono, monospace",
							fontSize: "12px",
							cursor: "pointer",
							fontWeight: 600,
						}}
					>
						Run
					</button>
				</div>
			</div>
		</div>
	);
}

// ── Main IDE Page ─────────────────────────────────────────────────────────────
export default function IDEPage() {
	const [tabs, setTabs] = useState<Tab[]>(DEFAULT_TABS);
	const [activeTabId, setActiveTabId] = useState("1");
	const [output, setOutput] = useState<OutputLine[]>([
		{
			id: "0",
			type: "system",
			text: "cxIDE ready. Press ▶ Run or Ctrl+Enter to execute.",
			timestamp: now(),
		},
	]);
	const [markers, setMarkers] = useState<DiagnosticMarker[]>([]);
	const [isRunning, setIsRunning] = useState(false);
	const [theme, setTheme] = useState<"dark" | "light">("dark");
	const [showSnippets, setShowSnippets] = useState(false);
	const [activePanel, setActivePanel] = useState<"output" | "problems">("output");
	const [cursorInfo, setCursorInfo] = useState({ line: 1, col: 1 });

	// Stdin modal state
	const [stdinModal, setStdinModal] = useState<{
		inputCount: number;
		resolve: (stdin: string) => void;
		reject: () => void;
	} | null>(null);

	const activeTab = tabs.find((t) => t.id === activeTabId) ?? tabs[0];

	// Prompts user for stdin input via modal, returns a Promise<string>
	const promptForStdin = (inputCount: number): Promise<string> => {
		return new Promise((resolve, reject) => {
			setStdinModal({ inputCount, resolve, reject });
		});
	};

	const updateCode = useCallback(
		(code: string) => {
			setTabs((prev) =>
				prev.map((t) =>
					t.id === activeTabId ? { ...t, code, isModified: true } : t,
				),
			);
		},
		[activeTabId],
	);

	const addTab = () => {
		const id = String(Date.now());
		const newTab: Tab = {
			id,
			name: `untitled-${tabs.length + 1}.cx`,
			code: "",
			isModified: false,
		};
		setTabs((prev) => [...prev, newTab]);
		setActiveTabId(id);
	};

	const closeTab = (id: string) => {
		if (tabs.length === 1) return;
		const idx = tabs.findIndex((t) => t.id === id);
		setTabs((prev) => prev.filter((t) => t.id !== id));
		setActiveTabId(tabs[idx === 0 ? 1 : idx - 1].id);
	};

	const renameTab = (id: string, name: string) => {
		setTabs((prev) => prev.map((t) => (t.id === id ? { ...t, name } : t)));
	};

	const runCode = async () => {
		if (isRunning) return;

		const code = activeTab.code;

		// Detect input() calls — prompt user for stdin if needed
		const inputMatches = code.match(/\binput\s*\(/g);
		const inputCount = inputMatches ? inputMatches.length : 0;

		let stdinData = "";
		if (inputCount > 0) {
			try {
				stdinData = await promptForStdin(inputCount);
			} catch {
				// User cancelled — don't run
				return;
			}
		}

		setIsRunning(true);
		setMarkers([]);

		const ts = now();
		setOutput((prev) => [
			...prev,
			{
				id: ts + "-run",
				type: "system",
				text: `▶ Running "${activeTab.name}"...`,
				timestamp: ts,
			},
		]);

		try {
			const res = await fetch("http://localhost:4000/api/run", {
				method: "POST",
				headers: { "Content-Type": "application/json" },
				body: JSON.stringify({
					code,
					filename: activeTab.name,
					stdin: stdinData,
				}),
			});

			const data = await res.json();

			if (data.stdout) {
				const lines: OutputLine[] = data.stdout
					.split("\n")
					.filter(Boolean)
					.map((line: string, i: number) => ({
						id: `${ts}-out-${i}`,
						type: "stdout" as const,
						text: line,
						timestamp: now(),
					}));
				setOutput((prev) => [...prev, ...lines]);
			}

			if (data.stderr) {
				const errLines: OutputLine[] = data.stderr
					.split("\n")
					.filter(Boolean)
					.map((line: string, i: number) => ({
						id: `${ts}-err-${i}`,
						type: "stderr" as const,
						text: line,
						timestamp: now(),
					}));
				setOutput((prev) => [...prev, ...errLines]);

				const parsed = parseErrors(data.stderr);
				setMarkers(parsed);
				if (parsed.length > 0) setActivePanel("problems");
			}

			if (!data.stdout && !data.stderr) {
				setOutput((prev) => [
					...prev,
					{
						id: ts + "-done",
						type: "system",
						text: "✓ Process exited with no output.",
						timestamp: now(),
					},
				]);
			} else {
				const exitLabel =
					data.exitCode === 0 ? "✓ Exited (0)" : `✗ Exited (${data.exitCode})`;
				setOutput((prev) => [
					...prev,
					{
						id: ts + "-exit",
						type: data.exitCode === 0 ? "system" : "stderr",
						text: exitLabel,
						timestamp: now(),
					},
				]);
			}
		} catch (err) {
			setOutput((prev) => [
				...prev,
				{
					id: ts + "-conn",
					type: "stderr",
					text: "✗ Could not connect to backend. Is the server running on port 4000?",
					timestamp: now(),
				},
			]);
		} finally {
			setIsRunning(false);
		}
	};

	const clearOutput = () => {
		setOutput([
			{
				id: "cleared",
				type: "system",
				text: "Output cleared.",
				timestamp: now(),
			},
		]);
		setMarkers([]);
	};

	const insertSnippet = (code: string) => {
		updateCode(activeTab.code + "\n\n" + code);
		setShowSnippets(false);
	};

	return (
		<div
			className={theme === "light" ? "theme-light" : ""}
			style={{
				height: "100vh",
				display: "flex",
				flexDirection: "column",
				background: "var(--bg-primary)",
				overflow: "hidden",
			}}
		>
			{/* Stdin modal — rendered above everything */}
			{stdinModal && (
				<StdinModal
					inputCount={stdinModal.inputCount}
					onSubmit={(stdin) => {
						stdinModal.resolve(stdin);
						setStdinModal(null);
					}}
					onCancel={() => {
						stdinModal.reject();
						setStdinModal(null);
					}}
				/>
			)}

			{/* Top toolbar */}
			<Toolbar
				isRunning={isRunning}
				onRun={runCode}
				theme={theme}
				onToggleTheme={() => setTheme((t) => (t === "dark" ? "light" : "dark"))}
				onToggleSnippets={() => setShowSnippets((s) => !s)}
				showSnippets={showSnippets}
			/>

			<div style={{ flex: 1, display: "flex", overflow: "hidden" }}>
				{/* Sidebar */}
				<Sidebar
					tabs={tabs}
					activeTabId={activeTabId}
					onSelectTab={setActiveTabId}
				/>

				{/* Main editor area */}
				<div
					style={{
						flex: 1,
						display: "flex",
						flexDirection: "column",
						overflow: "hidden",
					}}
				>
					{/* Tab bar */}
					<TabBar
						tabs={tabs}
						activeTabId={activeTabId}
						onSelectTab={setActiveTabId}
						onCloseTab={closeTab}
						onAddTab={addTab}
						onRenameTab={renameTab}
					/>

					{/* Editor */}
					<div style={{ flex: 1, overflow: "hidden", position: "relative" }}>
						<CodeEditor
							code={activeTab.code}
							onChange={updateCode}
							theme={theme}
							markers={markers}
							onCursorChange={setCursorInfo}
						/>
					</div>

					{/* Output panel */}
					<OutputPanel
						output={output}
						markers={markers}
						onClear={clearOutput}
						activePanel={activePanel}
						onSetPanel={setActivePanel}
					/>
				</div>

				{/* Snippets sidebar */}
				{showSnippets && (
					<SnippetsPanel
						onInsert={insertSnippet}
						onClose={() => setShowSnippets(false)}
					/>
				)}
			</div>

			{/* Status bar */}
			<StatusBar
				activeTab={activeTab}
				cursorInfo={cursorInfo}
				isRunning={isRunning}
				errorCount={markers.filter((m) => m.severity === "error").length}
				warnCount={markers.filter((m) => m.severity === "warning").length}
			/>
		</div>
	);
}

function now() {
	return new Date().toLocaleTimeString("en-US", { hour12: false });
}

function parseErrors(stderr: string): DiagnosticMarker[] {
	const markers: DiagnosticMarker[] = [];
	const patterns = [
		/(?:.*):(\d+):(\d+):\s*(error|warning):\s*(.+)/i,
		/(?:.*):(\d+):\s*(error|warning):\s*(.+)/i,
		/line\s+(\d+)[:,]\s*(.+)/i,
		/Error at line (\d+):\s*(.+)/i,
	];
	for (const line of stderr.split("\n")) {
		for (const pattern of patterns) {
			const m = line.match(pattern);
			if (m) {
				const lineNum = parseInt(m[1]);
				const col = m[2] && !isNaN(parseInt(m[2])) ? parseInt(m[2]) : 1;
				const sev = m[3]?.toLowerCase() === "warning" ? "warning" : "error";
				const msg = m[4] ?? m[3] ?? m[2] ?? "Unknown error";
				markers.push({
					startLineNumber: lineNum,
					startColumn: col,
					endLineNumber: lineNum,
					endColumn: col + 10,
					message: msg,
					severity: sev,
				});
				break;
			}
		}
	}
	return markers;
}