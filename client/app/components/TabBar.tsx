"use client";

import { useState } from "react";
import { X, Plus, FileCode2 } from "lucide-react";
import type { Tab } from "@/app/page";

type Props = {
	tabs: Tab[];
	activeTabId: string;
	onSelectTab: (id: string) => void;
	onCloseTab: (id: string) => void;
	onAddTab: () => void;
	onRenameTab: (id: string, name: string) => void;
};

export default function TabBar({
	tabs,
	activeTabId,
	onSelectTab,
	onCloseTab,
	onAddTab,
	onRenameTab,
}: Props) {
	const [editingId, setEditingId] = useState<string | null>(null);
	const [editValue, setEditValue] = useState("");

	const startEdit = (tab: Tab, e: React.MouseEvent) => {
		e.preventDefault();
		setEditingId(tab.id);
		setEditValue(tab.name);
	};

	const commitEdit = (id: string) => {
		if (editValue.trim()) onRenameTab(id, editValue.trim());
		setEditingId(null);
	};

	return (
		<div
			className="panel-border-bottom"
			style={{
				height: "36px",
				background: "var(--bg-secondary)",
				display: "flex",
				alignItems: "stretch",
				overflowX: "auto",
				overflowY: "hidden",
				flexShrink: 0,
			}}
		>
			{tabs.map((tab) => (
				<div
					key={tab.id}
					onClick={() => onSelectTab(tab.id)}
					onDoubleClick={(e) => startEdit(tab, e)}
					className={tab.id === activeTabId ? "tab-active" : "tab-inactive"}
					style={{
						display: "flex",
						alignItems: "center",
						gap: "6px",
						padding: "0 10px",
						cursor: "pointer",
						whiteSpace: "nowrap",
						flexShrink: 0,
						fontSize: "12px",
						fontFamily: "JetBrains Mono, monospace",
						minWidth: "100px",
						maxWidth: "180px",
						transition: "all 0.1s",
					}}
				>
					<FileCode2
						size={11}
						style={{ flexShrink: 0 }}
						color="var(--text-muted)"
					/>
					{editingId === tab.id ? (
						<input
							autoFocus
							value={editValue}
							onChange={(e) => setEditValue(e.target.value)}
							onBlur={() => commitEdit(tab.id)}
							onKeyDown={(e) => {
								if (e.key === "Enter") commitEdit(tab.id);
								if (e.key === "Escape") setEditingId(null);
							}}
							onClick={(e) => e.stopPropagation()}
							style={{
								background: "var(--bg-surface)",
								border: "1px solid var(--accent)",
								borderRadius: "3px",
								color: "var(--text-primary)",
								fontFamily: "JetBrains Mono, monospace",
								fontSize: "11px",
								padding: "1px 4px",
								width: "110px",
								outline: "none",
							}}
						/>
					) : (
						<span
							style={{ overflow: "hidden", textOverflow: "ellipsis", flex: 1 }}
						>
							{tab.name}
							{tab.isModified && (
								<span
									style={{
										color: "var(--warn)",
										marginLeft: "4px",
										fontSize: "14px",
										lineHeight: 0,
									}}
								>
									●
								</span>
							)}
						</span>
					)}
					<button
						onClick={(e) => {
							e.stopPropagation();
							onCloseTab(tab.id);
						}}
						style={{
							display: "flex",
							alignItems: "center",
							justifyContent: "center",
							width: "16px",
							height: "16px",
							borderRadius: "3px",
							border: "none",
							background: "transparent",
							color: "var(--text-muted)",
							cursor: "pointer",
							flexShrink: 0,
							opacity: 0.7,
							transition: "all 0.1s",
						}}
					>
						<X size={10} />
					</button>
				</div>
			))}

			<button
				onClick={onAddTab}
				style={{
					display: "flex",
					alignItems: "center",
					justifyContent: "center",
					padding: "0 10px",
					border: "none",
					background: "transparent",
					color: "var(--text-muted)",
					cursor: "pointer",
					transition: "all 0.15s",
					flexShrink: 0,
				}}
				title="New file"
			>
				<Plus size={14} />
			</button>
		</div>
	);
}
