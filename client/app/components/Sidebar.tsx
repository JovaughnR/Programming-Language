"use client";

import { FileCode2, Files } from "lucide-react";
import type { Tab } from "@/app/page";

type Props = {
	tabs: Tab[];
	activeTabId: string;
	onSelectTab: (id: string) => void;
};

export default function Sidebar({ tabs, activeTabId, onSelectTab }: Props) {
	return (
		<div
			className="panel-border-right"
			style={{
				width: "200px",
				background: "var(--bg-secondary)",
				display: "flex",
				flexDirection: "column",
				overflow: "hidden",
				flexShrink: 0,
			}}
		>
			{/* Section header */}
			<div
				style={{
					padding: "10px 12px 6px",
					display: "flex",
					alignItems: "center",
					gap: "6px",
				}}
			>
				<Files size={12} color="var(--text-muted)" />
				<span
					style={{
						fontFamily: "DM Sans, sans-serif",
						fontSize: "10px",
						fontWeight: 600,
						letterSpacing: "0.1em",
						textTransform: "uppercase",
						color: "var(--text-muted)",
					}}
				>
					Explorer
				</span>
			</div>

			{/* File list */}
			<div style={{ flex: 1, overflowY: "auto", paddingBottom: "8px" }}>
				{tabs.map((tab) => (
					<button
						key={tab.id}
						onClick={() => onSelectTab(tab.id)}
						style={{
							width: "100%",
							display: "flex",
							alignItems: "center",
							gap: "8px",
							padding: "6px 12px",
							border: "none",
							background:
								tab.id === activeTabId ? "var(--bg-surface)" : "transparent",
							borderLeft:
								tab.id === activeTabId
									? "2px solid var(--accent)"
									: "2px solid transparent",
							color:
								tab.id === activeTabId
									? "var(--text-primary)"
									: "var(--text-muted)",
							cursor: "pointer",
							textAlign: "left",
							transition: "all 0.1s",
							fontSize: "12px",
							fontFamily: "JetBrains Mono, monospace",
						}}
					>
						<FileCode2
							size={13}
							color={
								tab.id === activeTabId ? "var(--accent)" : "var(--text-muted)"
							}
						/>
						<span
							style={{
								overflow: "hidden",
								textOverflow: "ellipsis",
								whiteSpace: "nowrap",
								flex: 1,
							}}
						>
							{tab.name}
						</span>
						{tab.isModified && (
							<span
								style={{
									width: "6px",
									height: "6px",
									borderRadius: "50%",
									background: "var(--warn)",
									flexShrink: 0,
								}}
							/>
						)}
					</button>
				))}
			</div>
		</div>
	);
}
