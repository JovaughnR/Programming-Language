import type { Metadata } from "next";
import "./globals.css";

export const metadata: Metadata = {
	title: "LangIDE — Custom Language Playground",
	description: "Interactive IDE for your custom programming language",
};

export default function RootLayout({
	children,
}: {
	children: React.ReactNode;
}) {
	return (
		<html lang="en">
			<body>{children}</body>
		</html>
	);
}
