import * as monaco from "monaco-editor";

window.addEventListener(
    "DOMContentLoaded", () => {
        monaco.editor.create(
            document.getElementById("editor"), 
            {
                value: "print(\"Hello World!\")\n",
                language: "python",
                theme: "vs-dark",
                automaticLayout: true,
                fontSize: 15,
                fontWeight: "bold",
                minimap: { enabled: false },
            }
        );
    }
);
