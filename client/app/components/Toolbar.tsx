"use client";

import { Play, Loader2, Sun, Moon, BookOpen, Cpu } from "lucide-react";

type Props = {
	isRunning: boolean;
	onRun: () => void;
	theme: "dark" | "light";
	onToggleTheme: () => void;
	onToggleSnippets: () => void;
	showSnippets: boolean;
};

export default function Toolbar({
	isRunning,
	onRun,
	theme,
	onToggleTheme,
	onToggleSnippets,
	showSnippets,
}: Props) {
	return (
		<div
			className="panel-border-bottom"
			style={{
				height: "48px",
				background: "var(--bg-secondary)",
				display: "flex",
				alignItems: "center",
				padding: "0 12px",
				gap: "8px",
				userSelect: "none",
			}}
		>
			{/* Logo */}
			<div
				style={{
					display: "flex",
					alignItems: "center",
					gap: "8px",
					marginRight: "16px",
				}}
			>
				<div
					style={{
						width: "28px",
						height: "28px",
						background: "var(--accent)",
						borderRadius: "6px",
						display: "flex",
						alignItems: "center",
						justifyContent: "center",
					}}
				>
					<Cpu size={14} color="#000" strokeWidth={2.5} />
				</div>
				<span
					style={{
						fontFamily: "Syne, sans-serif",
						fontWeight: 700,
						fontSize: "15px",
						letterSpacing: "-0.02em",
						color: "var(--text-primary)",
					}}
				>
					Lang<span style={{ color: "var(--accent)" }}>IDE</span>
				</span>
			</div>

			{/* Divider */}
			<div
				style={{
					width: "1px",
					height: "24px",
					background: "var(--border)",
					margin: "0 4px",
				}}
			/>

			{/* Run button */}
			<button
				className="btn-run"
				onClick={onRun}
				disabled={isRunning}
				style={{
					display: "flex",
					alignItems: "center",
					gap: "6px",
					padding: "6px 14px",
					borderRadius: "6px",
					border: "none",
					fontFamily: "JetBrains Mono, monospace",
					fontSize: "12px",
					fontWeight: 600,
					letterSpacing: "0.02em",
					cursor: isRunning ? "not-allowed" : "pointer",
				}}
			>
				{isRunning ? (
					<Loader2 size={13} className="spin" />
				) : (
					<Play size={13} fill="currentColor" />
				)}
				{isRunning ? "Running..." : "Run"}
				<span style={{ opacity: 0.6, fontSize: "10px", marginLeft: "2px" }}>
					⌘↵
				</span>
			</button>

			{/* Spacer */}
			<div style={{ flex: 1 }} />

			{/* Snippets toggle */}
			<button
				onClick={onToggleSnippets}
				style={{
					display: "flex",
					alignItems: "center",
					gap: "6px",
					padding: "5px 10px",
					borderRadius: "6px",
					border: `1px solid ${showSnippets ? "var(--accent)" : "var(--border)"}`,
					background: showSnippets ? "rgba(0,255,156,0.08)" : "transparent",
					color: showSnippets ? "var(--accent)" : "var(--text-muted)",
					fontFamily: "DM Sans, sans-serif",
					fontSize: "12px",
					cursor: "pointer",
					transition: "all 0.15s",
				}}
			>
				<BookOpen size={13} />
				Snippets
			</button>

			{/* Theme toggle */}
			<button
				onClick={onToggleTheme}
				style={{
					display: "flex",
					alignItems: "center",
					justifyContent: "center",
					width: "32px",
					height: "32px",
					borderRadius: "6px",
					border: "1px solid var(--border)",
					background: "transparent",
					color: "var(--text-muted)",
					cursor: "pointer",
					transition: "all 0.15s",
				}}
				title="Toggle theme"
			>
				{theme === "dark" ? <Sun size={14} /> : <Moon size={14} />}
			</button>
		</div>
	);
}
