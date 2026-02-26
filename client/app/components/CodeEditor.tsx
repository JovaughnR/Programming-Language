"use client";

import { useEffect, useRef, useCallback } from "react";
import Editor, { useMonaco } from "@monaco-editor/react";
import type { DiagnosticMarker } from "@/app/page";
import type * as Monaco from "monaco-editor";

type Props = {
	code: string;
	onChange: (code: string) => void;
	theme: "dark" | "light";
	markers: DiagnosticMarker[];
	onCursorChange: (info: { line: number; col: number }) => void;
};

const LANG_ID = "customlang";

// ── Control flow ─────────────────────────────────────── coral/pink
const LANG_CONTROL = [
	"return",
	"break",
	"continue",
	"pass",
	"try",
	"catch",
	"throw",
	"finally",
];

// ── Structure / declaration keywords ────────────────── purple
const LANG_KEYWORDS = [
	"func",
	"if",
	"elif",
	"else",
	"while",
	"for",
	"in",
	"import",
	"from",
	"as",
	"class",
	"this",
	"global",
	"nonlocal",
];

// ── Boolean / null constants ─────────────────────────── orange
const LANG_CONSTANTS = ["true", "false", "none"];

// ── Logical operators (word form) ────────────────────── purple italic
const LANG_LOGICAL = ["and", "or", "not"];

// ── Type builtins ─────────────────────────────────────── yellow
const LANG_TYPES = ["int", "float", "str", "bool", "list", "dict", "set"];

// ── Function builtins ─────────────────────────────────── blue
const LANG_BUILTINS = [
	"print",
	"input",
	"len",
	"type",
	"range",
	"abs",
	"min",
	"max",
	"sum",
	"round",
	"sorted",
];

function registerLanguage(monaco: typeof Monaco) {
	if (monaco.languages.getLanguages().some((l) => l.id === LANG_ID)) return;

	monaco.languages.register({ id: LANG_ID });

	monaco.languages.setMonarchTokensProvider(LANG_ID, {
		keywords: LANG_KEYWORDS,
		control: LANG_CONTROL,
		constants: LANG_CONSTANTS,
		logical: LANG_LOGICAL,
		types: LANG_TYPES,
		builtins: LANG_BUILTINS,

		operators: [
			"=",
			"+",
			"-",
			"*",
			"/",
			"%",
			"==",
			"!=",
			"<",
			">",
			"<=",
			">=",
			"+=",
			"-=",
			"*=",
			"/=",
			"%=",
			"&=",
			"|=",
			"^=",
		],
		symbols: /[=+\-*\/%<>!&|,;:.()[\]{}]+/,

		tokenizer: {
			root: [
				// Comments
				[/\/\/.*$/, "comment"],
				[/\/\*/, { token: "comment.block", next: "@blockComment" }],
				[/#.*$/, "comment"],

				// Strings
				[/"([^"\\]|\\.)*$/, "string.invalid"],
				[/"/, { token: "string.quote", bracket: "@open", next: "@string" }],
				[/'([^'\\]|\\.)*$/, "string.invalid"],
				[/'/, { token: "string.quote", bracket: "@open", next: "@stringS" }],

				// Numbers
				[/\d*\.\d+([eE][-+]?\d+)?/, "number.float"],
				[/0[xX][0-9a-fA-F]+/, "number.hex"],
				[/\d+/, "number"],

				// this.field → keyword + field as object property
				[/this(?=\s*\.)/, "keyword"],
				[/(?<=this\.)[a-zA-Z_$]\w*/, "variable.field"],

				// Function definition name: func <name>(
				[/(?<=func\s+)[a-zA-Z_$]\w*(?=\s*\()/, "function.def"],

				// Function calls — word immediately followed by (
				[
					/[a-zA-Z_$]\w*(?=\s*\()/,
					{
						cases: {
							"@builtins": "predefined",
							"@types": "type.call",
							"@keywords": "keyword",
							"@control": "control",
							"@default": "function.call",
						},
					},
				],

				// Variable assignment — word followed by = (not ==)
				[
					/[a-zA-Z_$]\w*(?=\s*=[^=])/,
					{
						cases: {
							"@keywords": "keyword",
							"@control": "control",
							"@constants": "constant",
							"@logical": "logical",
							"@types": "type.name",
							"@builtins": "predefined",
							"@default": "variable.assignment",
						},
					},
				],

				// All other identifiers (variable reads, params, etc.)
				[
					/[a-zA-Z_$]\w*/,
					{
						cases: {
							"@keywords": "keyword",
							"@control": "control",
							"@constants": "constant",
							"@logical": "logical",
							"@types": "type.name",
							"@builtins": "predefined",
							"@default": "variable.other",
						},
					},
				],

				// Operators
				[/@symbols/, { cases: { "@operators": "operator", "@default": "" } }],

				// Delimiters
				[/[{}()[\]]/, "delimiter.bracket"],
				[/[,;:]/, "delimiter"],

				// Whitespace
				[/\s+/, "white"],
			],

			string: [
				[/[^\\"]+/, "string"],
				[/\\./, "string.escape"],
				[/"/, { token: "string.quote", bracket: "@close", next: "@pop" }],
			],
			stringS: [
				[/[^\\']+/, "string"],
				[/\\./, "string.escape"],
				[/'/, { token: "string.quote", bracket: "@close", next: "@pop" }],
			],
			blockComment: [
				[/[^/*]+/, "comment.block"],
				[/\*\//, { token: "comment.block", next: "@pop" }],
				[/[/*]/, "comment.block"],
			],
		},
	});

	monaco.languages.setLanguageConfiguration(LANG_ID, {
		comments: { lineComment: "#", blockComment: ["/*", "*/"] },
		brackets: [
			["{", "}"],
			["[", "]"],
			["(", ")"],
		],
		autoClosingPairs: [
			{ open: "{", close: "}" },
			{ open: "[", close: "]" },
			{ open: "(", close: ")" },
			{ open: '"', close: '"' },
			{ open: "'", close: "'" },
		],
		surroundingPairs: [
			{ open: "{", close: "}" },
			{ open: "[", close: "]" },
			{ open: "(", close: ")" },
			{ open: '"', close: '"' },
			{ open: "'", close: "'" },
		],
		indentationRules: {
			increaseIndentPattern: /^.*\{[^}]*$/,
			decreaseIndentPattern: /^\s*\}/,
		},
	});

	// ── Autocomplete ──────────────────────────────────────────────────────────
	monaco.languages.registerCompletionItemProvider(LANG_ID, {
		provideCompletionItems(model, position) {
			const word = model.getWordUntilPosition(position);
			const range = {
				startLineNumber: position.lineNumber,
				endLineNumber: position.lineNumber,
				startColumn: word.startColumn,
				endColumn: word.endColumn,
			};
			const suggestions = [
				...[...LANG_KEYWORDS, ...LANG_CONTROL, ...LANG_LOGICAL].map((kw) => ({
					label: kw,
					kind: monaco.languages.CompletionItemKind.Keyword,
					insertText: kw,
					range,
				})),
				...LANG_CONSTANTS.map((c) => ({
					label: c,
					kind: monaco.languages.CompletionItemKind.Constant,
					insertText: c,
					range,
				})),
				...LANG_TYPES.map((t) => ({
					label: t,
					kind: monaco.languages.CompletionItemKind.Class,
					insertText: `${t}($0)`,
					insertTextRules:
						monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
					range,
				})),
				...LANG_BUILTINS.map((b) => ({
					label: b,
					kind: monaco.languages.CompletionItemKind.Function,
					insertText: `${b}($0)`,
					insertTextRules:
						monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
					range,
				})),
				{
					label: "func",
					kind: monaco.languages.CompletionItemKind.Snippet,
					insertText: "func ${1:name}(${2:params}) {\n\t$0\n}",
					insertTextRules:
						monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
					documentation: "Function declaration",
					range,
				},
				{
					label: "if",
					kind: monaco.languages.CompletionItemKind.Snippet,
					insertText: "if (${1:condition}) {\n\t$0\n}",
					insertTextRules:
						monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
					documentation: "If statement",
					range,
				},
				{
					label: "while",
					kind: monaco.languages.CompletionItemKind.Snippet,
					insertText: "while (${1:condition}) {\n\t$0\n}",
					insertTextRules:
						monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
					documentation: "While loop",
					range,
				},
			];
			return { suggestions };
		},
	});

	// ── Dark theme (One Dark) ─────────────────────────────────────────────────
	monaco.editor.defineTheme("langide-dark", {
		base: "vs-dark",
		inherit: true,
		rules: [
			// Comments
			{ token: "comment", foreground: "5C6370", fontStyle: "italic" },
			{ token: "comment.block", foreground: "5C6370", fontStyle: "italic" },

			// Keywords — purple
			{ token: "keyword", foreground: "C792EA", fontStyle: "bold" },

			// Control flow — coral
			{ token: "control", foreground: "F07178", fontStyle: "bold" },

			// Logical operators — purple italic
			{ token: "logical", foreground: "C792EA", fontStyle: "italic" },

			// Constants true/false/none — orange
			{ token: "constant", foreground: "F78C6C" },

			// Types — yellow
			{ token: "type.name", foreground: "FFCB6B" },
			{ token: "type.call", foreground: "FFCB6B" },

			// Built-in functions — blue
			{ token: "predefined", foreground: "82AAFF" },

			// User function definition name — bright green bold
			{ token: "function.def", foreground: "C3E88D", fontStyle: "bold" },

			// User function calls — cyan
			{ token: "function.call", foreground: "89DDFF" },

			// Variable being written (x = ...) — rose/red
			{ token: "variable.assignment", foreground: "FF9CAC" },

			// Object fields (this.x) — gold
			{ token: "variable.field", foreground: "FFCB6B" },

			// Variable reads / params / everything else — soft lavender
			{ token: "variable.other", foreground: "D6ACFF" },

			// Strings
			{ token: "string", foreground: "98C379" },
			{ token: "string.quote", foreground: "98C379" },
			{ token: "string.escape", foreground: "56B6C2" },
			{ token: "string.invalid", foreground: "F07178" },

			// Numbers
			{ token: "number", foreground: "D19A66" },
			{ token: "number.float", foreground: "D19A66" },
			{ token: "number.hex", foreground: "D19A66" },

			// Operators — cyan
			{ token: "operator", foreground: "56B6C2" },

			// Delimiters
			{ token: "delimiter", foreground: "ABB2BF" },
			{ token: "delimiter.bracket", foreground: "C8C8C8" },
		],
		colors: {
			"editor.background": "#161B22",
			"editor.foreground": "#C9D1D9",
			"editorLineNumber.foreground": "#3D444D",
			"editorLineNumber.activeForeground": "#00FF9C",
			"editor.lineHighlightBackground": "#1F2937",
			"editorCursor.foreground": "#00FF9C",
			"editor.selectionBackground": "#264F78",
			"editorIndentGuide.background": "#21262D",
			"editorIndentGuide.activeBackground": "#30363D",
			"editorBracketMatch.background": "#00FF9C22",
			"editorBracketMatch.border": "#00FF9C",
			"scrollbarSlider.background": "#30363D80",
			"scrollbarSlider.hoverBackground": "#484F5880",
			"minimap.background": "#0D1117",
		},
	});

	// ── Light theme ───────────────────────────────────────────────────────────
	monaco.editor.defineTheme("langide-light", {
		base: "vs",
		inherit: true,
		rules: [
			{ token: "comment", foreground: "6A737D", fontStyle: "italic" },
			{ token: "comment.block", foreground: "6A737D", fontStyle: "italic" },
			{ token: "keyword", foreground: "A626A4", fontStyle: "bold" },
			{ token: "control", foreground: "E45649", fontStyle: "bold" },
			{ token: "logical", foreground: "A626A4", fontStyle: "italic" },
			{ token: "constant", foreground: "986801" },
			{ token: "type.name", foreground: "C18401" },
			{ token: "type.call", foreground: "C18401" },
			{ token: "predefined", foreground: "4078F2" },
			{ token: "function.def", foreground: "4078F2", fontStyle: "bold" },
			{ token: "function.call", foreground: "0184BC" },
			{ token: "variable.assignment", foreground: "E45649" },
			{ token: "variable.field", foreground: "986801" },
			{ token: "variable.other", foreground: "7B36B8" },
			{ token: "string", foreground: "50A14F" },
			{ token: "string.quote", foreground: "50A14F" },
			{ token: "string.escape", foreground: "0184BC" },
			{ token: "string.invalid", foreground: "E45649" },
			{ token: "number", foreground: "986801" },
			{ token: "number.float", foreground: "986801" },
			{ token: "number.hex", foreground: "986801" },
			{ token: "operator", foreground: "0184BC" },
			{ token: "delimiter", foreground: "383A42" },
			{ token: "delimiter.bracket", foreground: "383A42" },
		],
		colors: {
			"editor.background": "#F6F8FA",
			"editor.foreground": "#383A42",
			"editorLineNumber.foreground": "#C6CBD1",
			"editorLineNumber.activeForeground": "#0D7A4E",
			"editorCursor.foreground": "#0D7A4E",
			"editor.selectionBackground": "#C8E6C9",
			"editorBracketMatch.background": "#0D7A4E22",
			"editorBracketMatch.border": "#0D7A4E",
		},
	});
}

export default function CodeEditor({
	code,
	onChange,
	theme,
	markers,
	onCursorChange,
}: Props) {
	const monaco = useMonaco();
	const editorRef = useRef<Monaco.editor.IStandaloneCodeEditor | null>(null);

	useEffect(() => {
		if (!monaco) return;
		registerLanguage(monaco);
	}, [monaco]);

	useEffect(() => {
		if (!monaco || !editorRef.current) return;
		const model = editorRef.current.getModel();
		if (!model) return;

		const monacoMarkers = markers.map((m) => ({
			startLineNumber: m.startLineNumber,
			startColumn: m.startColumn,
			endLineNumber: m.endLineNumber,
			endColumn: m.endColumn,
			message: m.message,
			severity:
				m.severity === "error"
					? monaco.MarkerSeverity.Error
					: monaco.MarkerSeverity.Warning,
		}));
		monaco.editor.setModelMarkers(model, LANG_ID, monacoMarkers);
	}, [markers, monaco]);

	const handleMount = useCallback(
		(editor: Monaco.editor.IStandaloneCodeEditor) => {
			editorRef.current = editor;
			editor.onDidChangeCursorPosition((e) => {
				onCursorChange({ line: e.position.lineNumber, col: e.position.column });
			});
		},
		[onCursorChange],
	);

	if (!monaco)
		return (
			<div
				style={{
					height: "100%",
					background: "var(--bg-panel)",
					display: "flex",
					alignItems: "center",
					justifyContent: "center",
				}}
			>
				<span
					style={{
						color: "var(--text-muted)",
						fontFamily: "JetBrains Mono, monospace",
						fontSize: "13px",
					}}
				>
					Loading editor...
				</span>
			</div>
		);

	return (
		<Editor
			height="100%"
			language={LANG_ID}
			value={code}
			theme={theme === "dark" ? "langide-dark" : "langide-light"}
			onChange={(val) => onChange(val ?? "")}
			onMount={handleMount}
			options={{
				fontFamily: '"JetBrains Mono", "Fira Code", monospace',
				fontLigatures: true,
				fontSize: 14,
				lineHeight: 22,
				minimap: { enabled: true, scale: 1 },
				scrollBeyondLastLine: false,
				roundedSelection: true,
				cursorBlinking: "smooth",
				cursorSmoothCaretAnimation: "on",
				smoothScrolling: true,
				bracketPairColorization: { enabled: true },
				guides: { bracketPairs: true, indentation: true },
				renderLineHighlight: "all",
				padding: { top: 12, bottom: 12 },
				overviewRulerLanes: 3,
				codeLens: false,
				quickSuggestions: { other: true, comments: false, strings: false },
				wordBasedSuggestions: "currentDocument",
				tabSize: 2,
				insertSpaces: true,
				autoIndent: "full",
				folding: true,
				showFoldingControls: "mouseover",
				scrollbar: {
					vertical: "auto",
					horizontal: "auto",
					verticalScrollbarSize: 6,
					horizontalScrollbarSize: 6,
				},
			}}
		/>
	);
}
