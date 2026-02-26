"use client";

import { useEffect, useRef } from "react";
import { Trash2, AlertTriangle, CheckCircle, Terminal } from "lucide-react";
import type { OutputLine, DiagnosticMarker } from "@/app/page";

type Props = {
	output: OutputLine[];
	markers: DiagnosticMarker[];
	onClear: () => void;
	activePanel: "output" | "problems";
	onSetPanel: (p: "output" | "problems") => void;
};

export default function OutputPanel({
	output,
	markers,
	onClear,
	activePanel,
	onSetPanel,
}: Props) {
	const bottomRef = useRef<HTMLDivElement>(null);

	useEffect(() => {
		bottomRef.current?.scrollIntoView({ behavior: "smooth" });
	}, [output]);

	const errorCount = markers.filter((m) => m.severity === "error").length;
	const warnCount = markers.filter((m) => m.severity === "warning").length;

	return (
		<div
			className="panel-border-top"
			style={{
				height: "220px",
				background: "var(--bg-secondary)",
				display: "flex",
				flexDirection: "column",
				flexShrink: 0,
			}}
		>
			{/* Panel tabs */}
			<div
				className="panel-border-bottom"
				style={{
					height: "32px",
					display: "flex",
					alignItems: "stretch",
					paddingLeft: "8px",
				}}
			>
				{[
					{ id: "output", label: "Output", icon: <Terminal size={11} /> },
					{
						id: "problems",
						label: `Problems${errorCount + warnCount > 0 ? ` (${errorCount + warnCount})` : ""}`,
						icon: <AlertTriangle size={11} />,
					},
				].map((tab) => (
					<button
						key={tab.id}
						onClick={() => onSetPanel(tab.id as "output" | "problems")}
						style={{
							display: "flex",
							alignItems: "center",
							gap: "5px",
							padding: "0 12px",
							border: "none",
							borderBottom:
								activePanel === tab.id
									? "2px solid var(--accent)"
									: "2px solid transparent",
							background: "transparent",
							color:
								activePanel === tab.id
									? "var(--text-primary)"
									: "var(--text-muted)",
							cursor: "pointer",
							fontFamily: "DM Sans, sans-serif",
							fontSize: "11px",
							fontWeight: activePanel === tab.id ? 500 : 400,
							transition: "all 0.1s",
						}}
					>
						{tab.icon}
						{tab.label}
					</button>
				))}

				<div style={{ flex: 1 }} />

				<button
					onClick={onClear}
					title="Clear output"
					style={{
						display: "flex",
						alignItems: "center",
						justifyContent: "center",
						width: "32px",
						border: "none",
						background: "transparent",
						color: "var(--text-muted)",
						cursor: "pointer",
					}}
				>
					<Trash2 size={12} />
				</button>
			</div>

			{/* Content */}
			<div style={{ flex: 1, overflowY: "auto", padding: "8px 0" }}>
				{activePanel === "output" ? (
					<div
						style={{
							fontFamily: "JetBrains Mono, monospace",
							fontSize: "12px",
							lineHeight: "1.7",
						}}
					>
						{output.map((line) => (
							<div
								key={line.id}
								className="output-line"
								style={{
									display: "flex",
									alignItems: "baseline",
									gap: "8px",
									padding: "1px 16px",
									color: lineColor(line.type),
									background:
										line.type === "stderr"
											? "rgba(255,107,53,0.04)"
											: "transparent",
								}}
							>
								<span
									style={{
										color: "var(--text-muted)",
										fontSize: "10px",
										flexShrink: 0,
										opacity: 0.7,
									}}
								>
									{line.timestamp}
								</span>
								<span
									style={{ whiteSpace: "pre-wrap", wordBreak: "break-all" }}
								>
									{line.text}
								</span>
							</div>
						))}
						<div ref={bottomRef} />
					</div>
				) : (
					<div style={{ padding: "4px 0" }}>
						{markers.length === 0 ? (
							<div
								style={{
									display: "flex",
									alignItems: "center",
									gap: "8px",
									padding: "16px",
									color: "var(--text-muted)",
									fontFamily: "DM Sans, sans-serif",
									fontSize: "12px",
								}}
							>
								<CheckCircle size={14} color="var(--accent)" />
								No problems detected
							</div>
						) : (
							markers.map((m, i) => (
								<div
									key={i}
									style={{
										display: "flex",
										alignItems: "flex-start",
										gap: "8px",
										padding: "5px 16px",
										borderLeft: `2px solid ${m.severity === "error" ? "var(--error)" : "var(--warn)"}`,
										marginLeft: "8px",
										marginBottom: "2px",
										background:
											m.severity === "error"
												? "rgba(255,107,53,0.04)"
												: "rgba(255,215,0,0.04)",
										borderRadius: "0 4px 4px 0",
									}}
								>
									<AlertTriangle
										size={12}
										color={
											m.severity === "error" ? "var(--error)" : "var(--warn)"
										}
										style={{ flexShrink: 0, marginTop: "2px" }}
									/>
									<div>
										<div
											style={{
												fontFamily: "JetBrains Mono, monospace",
												fontSize: "12px",
												color:
													m.severity === "error"
														? "var(--error)"
														: "var(--warn)",
											}}
										>
											{m.message}
										</div>
										<div
											style={{
												fontFamily: "DM Sans, sans-serif",
												fontSize: "11px",
												color: "var(--text-muted)",
												marginTop: "2px",
											}}
										>
											Line {m.startLineNumber}, Col {m.startColumn}
										</div>
									</div>
								</div>
							))
						)}
					</div>
				)}
			</div>
		</div>
	);
}

function lineColor(type: OutputLine["type"]) {
	switch (type) {
		case "stderr":
			return "var(--error)";
		case "system":
			return "var(--text-muted)";
		case "info":
			return "var(--info)";
		default:
			return "var(--text-primary)";
	}
}
