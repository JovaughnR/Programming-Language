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
		name: "main.lang",
		isModified: false,
		code: `# Welcome to LangIDE
# Write your custom language code here

func greet(name):
  print("Hello, " + name + "!")


greet("World")
`,
	},
	{
		id: "2",
		name: "examples.lang",
		isModified: false,
		code: `# Fibonacci example
func fib(n):
  if (n <= 1) return n
  return fib(n - 1) + fib(n - 2)


i = 0
while (i < 10) :
  print(fib(i))
  i = i + 1

`,
	},
];

export default function IDEPage() {
	const [tabs, setTabs] = useState<Tab[]>(DEFAULT_TABS);
	const [activeTabId, setActiveTabId] = useState("1");
	const [output, setOutput] = useState<OutputLine[]>([
		{
			id: "0",
			type: "system",
			text: "LangIDE ready. Press ▶ Run or Ctrl+Enter to execute.",
			timestamp: now(),
		},
	]);
	const [markers, setMarkers] = useState<DiagnosticMarker[]>([]);
	const [isRunning, setIsRunning] = useState(false);
	const [theme, setTheme] = useState<"dark" | "light">("dark");
	const [showSnippets, setShowSnippets] = useState(false);
	const [activePanel, setActivePanel] = useState<"output" | "problems">(
		"output",
	);
	const [cursorInfo, setCursorInfo] = useState({ line: 1, col: 1 });

	const activeTab = tabs.find((t) => t.id === activeTabId) ?? tabs[0];

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
			name: `untitled-${tabs.length + 1}.lang`,
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
					code: activeTab.code,
					filename: activeTab.name,
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

				// Parse error markers from stderr
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
	// Handles common error formats:
	//   filename:line:col: error: message
	//   line N: message
	//   Error at line N: message
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
