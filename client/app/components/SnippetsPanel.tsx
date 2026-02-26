"use client";

import { X, ChevronRight } from "lucide-react";
import { useState } from "react";

type Snippet = {
	name: string;
	description: string;
	code: string;
	category: string;
};

const SNIPPETS: Snippet[] = [
	{
		category: "Functions",
		name: "Function Declaration",
		description: "Basic function with parameters",
		code: `func greet(name) {
  print("Hello, " + name + "!")
}

greet("World")`,
	},
	{
		category: "Functions",
		name: "Recursive Function",
		description: "Fibonacci via recursion",
		code: `func fib(n) :
  if (n <= 1) return n
  return fib(n - 1) + fib(n - 2)


print(fib(10))`,
	},
	{
		category: "Control Flow",
		name: "If / Else",
		description: "Conditional branching",
		code: `var x = 10

if (x > 5) :
  print("x is greater than 5")
else:
  print("x is 5 or less")
`,
	},
	{
		category: "Control Flow",
		name: "While Loop",
		description: "Basic while loop",
		code: `i = 0
while (i < 5):
  print(i)
  i = i + 1
`,
	},
	{
		category: "Control Flow",
		name: "For Loop",
		description: "Iterating with for",
		code: `for (var i = 0; i < 10; i = i + 1) {
  print(i)
}`,
	},
	{
		category: "Data",
		name: "Variables",
		description: "Declaring and using variables",
		code: `var name = "Alice"
var age = 30
var pi = 3.14159
var active = true

print(name + " is " + age + " years old")`,
	},
	{
		category: "Data",
		name: "String Operations",
		description: "String concatenation and printing",
		code: `var first = "Hello"
var second = "World"
var combined = first + ", " + second + "!"
print(combined)
print(len(combined))`,
	},
	{
		category: "Algorithms",
		name: "Bubble Sort",
		description: "Classic sorting algorithm",
		code: `func bubbleSort(arr) {
  var n = len(arr)
  var i = 0
  while (i < n) {
    var j = 0
    while (j < n - i - 1) {
      if (arr[j] > arr[j + 1]) {
        var temp = arr[j]
        arr[j] = arr[j + 1]
        arr[j + 1] = temp
      }
      j = j + 1
    }
    i = i + 1
  }
  return arr
}`,
	},
];

const CATEGORIES = [...new Set(SNIPPETS.map((s) => s.category))];

type Props = {
	onInsert: (code: string) => void;
	onClose: () => void;
};

export default function SnippetsPanel({ onInsert, onClose }: Props) {
	const [selectedCategory, setSelectedCategory] = useState<string>(
		CATEGORIES[0],
	);
	const [hoveredIdx, setHoveredIdx] = useState<number | null>(null);

	const filtered = SNIPPETS.filter((s) => s.category === selectedCategory);

	return (
		<div
			className="panel-border-left"
			style={{
				width: "260px",
				background: "var(--bg-secondary)",
				display: "flex",
				flexDirection: "column",
				flexShrink: 0,
				overflow: "hidden",
				animation: "fadeIn 0.2s ease forwards",
			}}
		>
			{/* Header */}
			<div
				className="panel-border-bottom"
				style={{
					padding: "10px 12px",
					display: "flex",
					alignItems: "center",
					justifyContent: "space-between",
				}}
			>
				<span
					style={{
						fontFamily: "Syne, sans-serif",
						fontWeight: 700,
						fontSize: "13px",
						color: "var(--text-primary)",
						letterSpacing: "-0.01em",
					}}
				>
					Code Snippets
				</span>
				<button
					onClick={onClose}
					style={{
						display: "flex",
						alignItems: "center",
						justifyContent: "center",
						width: "24px",
						height: "24px",
						borderRadius: "4px",
						border: "none",
						background: "transparent",
						color: "var(--text-muted)",
						cursor: "pointer",
					}}
				>
					<X size={13} />
				</button>
			</div>

			{/* Category pills */}
			<div
				className="panel-border-bottom"
				style={{
					display: "flex",
					flexWrap: "wrap",
					gap: "4px",
					padding: "8px",
				}}
			>
				{CATEGORIES.map((cat) => (
					<button
						key={cat}
						onClick={() => setSelectedCategory(cat)}
						style={{
							padding: "3px 8px",
							borderRadius: "12px",
							border: "none",
							background:
								selectedCategory === cat
									? "var(--accent)"
									: "var(--bg-surface)",
							color: selectedCategory === cat ? "#000" : "var(--text-muted)",
							fontFamily: "DM Sans, sans-serif",
							fontSize: "11px",
							fontWeight: selectedCategory === cat ? 600 : 400,
							cursor: "pointer",
							transition: "all 0.15s",
						}}
					>
						{cat}
					</button>
				))}
			</div>

			{/* Snippet list */}
			<div style={{ flex: 1, overflowY: "auto", padding: "6px" }}>
				{filtered.map((snippet, i) => (
					<button
						key={i}
						onClick={() => onInsert(snippet.code)}
						onMouseEnter={() => setHoveredIdx(i)}
						onMouseLeave={() => setHoveredIdx(null)}
						style={{
							width: "100%",
							textAlign: "left",
							padding: "10px",
							borderRadius: "6px",
							border: "1px solid transparent",
							background:
								hoveredIdx === i ? "var(--bg-surface)" : "transparent",
							borderColor: hoveredIdx === i ? "var(--border)" : "transparent",
							cursor: "pointer",
							marginBottom: "2px",
							transition: "all 0.1s",
							display: "flex",
							alignItems: "center",
							gap: "8px",
						}}
					>
						<div style={{ flex: 1, minWidth: 0 }}>
							<div
								style={{
									fontFamily: "JetBrains Mono, monospace",
									fontSize: "12px",
									color: "var(--text-primary)",
									fontWeight: 500,
									marginBottom: "2px",
								}}
							>
								{snippet.name}
							</div>
							<div
								style={{
									fontFamily: "DM Sans, sans-serif",
									fontSize: "11px",
									color: "var(--text-muted)",
									whiteSpace: "nowrap",
									overflow: "hidden",
									textOverflow: "ellipsis",
								}}
							>
								{snippet.description}
							</div>
						</div>
						<ChevronRight
							size={12}
							color={hoveredIdx === i ? "var(--accent)" : "var(--border)"}
							style={{ flexShrink: 0, transition: "color 0.1s" }}
						/>
					</button>
				))}
			</div>

			<div
				style={{
					padding: "8px 12px",
					borderTop: "1px solid var(--border)",
					fontFamily: "DM Sans, sans-serif",
					fontSize: "10px",
					color: "var(--text-muted)",
				}}
			>
				Click a snippet to insert at cursor
			</div>
		</div>
	);
}
