"use client";

import { Loader2, AlertTriangle, CheckCircle } from "lucide-react";
import type { Tab } from "@/app/page";

type Props = {
	activeTab: Tab;
	cursorInfo: { line: number; col: number };
	isRunning: boolean;
	errorCount: number;
	warnCount: number;
};

export default function StatusBar({
	activeTab,
	cursorInfo,
	isRunning,
	errorCount,
	warnCount,
}: Props) {
	const hasErrors = errorCount > 0;

	return (
		<div
			style={{
				height: "24px",
				background: hasErrors ? "rgba(255,107,53,0.15)" : "var(--bg-panel)",
				borderTop: `1px solid ${hasErrors ? "rgba(255,107,53,0.3)" : "var(--border)"}`,
				display: "flex",
				alignItems: "center",
				padding: "0 12px",
				gap: "16px",
				fontFamily: "JetBrains Mono, monospace",
				fontSize: "11px",
				color: "var(--text-muted)",
				userSelect: "none",
				flexShrink: 0,
				transition: "all 0.2s",
			}}
		>
			{/* Status */}
			<div style={{ display: "flex", alignItems: "center", gap: "6px" }}>
				{isRunning ? (
					<>
						<Loader2 size={10} className="spin" color="var(--warn)" />
						<span style={{ color: "var(--warn)" }}>Running...</span>
					</>
				) : hasErrors ? (
					<>
						<div className="status-dot error" />
						<span style={{ color: "var(--error)" }}>Errors</span>
					</>
				) : (
					<>
						<div className="status-dot" />
						<span style={{ color: "var(--accent)" }}>Ready</span>
					</>
				)}
			</div>

			<div
				style={{ width: "1px", height: "12px", background: "var(--border)" }}
			/>

			<span>{activeTab.name}</span>

			<div style={{ flex: 1 }} />

			{errorCount > 0 && (
				<div
					style={{
						display: "flex",
						alignItems: "center",
						gap: "4px",
						color: "var(--error)",
					}}
				>
					<AlertTriangle size={10} />
					{errorCount} error{errorCount !== 1 ? "s" : ""}
				</div>
			)}
			{warnCount > 0 && (
				<div
					style={{
						display: "flex",
						alignItems: "center",
						gap: "4px",
						color: "var(--warn)",
					}}
				>
					<AlertTriangle size={10} />
					{warnCount} warning{warnCount !== 1 ? "s" : ""}
				</div>
			)}
			{!hasErrors && warnCount === 0 && (
				<div
					style={{
						display: "flex",
						alignItems: "center",
						gap: "4px",
						color: "var(--accent)",
						opacity: 0.7,
					}}
				>
					<CheckCircle size={10} />
					No issues
				</div>
			)}

			<div
				style={{ width: "1px", height: "12px", background: "var(--border)" }}
			/>

			<span>
				Ln {cursorInfo.line}, Col {cursorInfo.col}
			</span>

			<div
				style={{ width: "1px", height: "12px", background: "var(--border)" }}
			/>

			<span style={{ color: "var(--accent)", opacity: 0.8 }}>
				Custom Language
			</span>
		</div>
	);
}
