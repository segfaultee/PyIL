import * as monaco from "monaco-editor";

let messageId = 0;
const pendingResponses = new Map();

window.electronAPI.onInitEditor(() => {
    const container = document.getElementById("editor");
    if (!container) {
        console.error("Editor container not found!");
        return;
    }
    
    const editor = monaco.editor.create(container, {
        value: "print(\"Hello World!\")",
        language: "python",
        theme: "vs-dark",
        automaticLayout: true,
        fontSize: 15,
        fontWeight: "bold",
        minimap: { enabled: false },
    });
    
    window.LSPAPI.onLSPMessage((message) => {
        if (message.id && pendingResponses.has(message.id)) {
            const { resolve } = pendingResponses.get(message.id);
            resolve(message);
            pendingResponses.delete(message.id);
        } else if (message.method === "window/showMessage") {
            console.log("LSP message:", message.params.message);
        }
    });
    
    function sendRequest(method, params) {
        return new Promise((resolve) => {
            const id = ++messageId;
            pendingResponses.set(id, { resolve });
            window.LSPAPI.sendLSPMessage({ jsonrpc: "2.0", id, method, params });
        });
    }
    
    sendRequest("initialize", {
        processId: null,
        rootUri: null,
        capabilities: {},
        workspaceFolders: null,
    }).then(() => {
        window.LSPAPI.sendLSPMessage({ jsonrpc: "2.0", method: "initialized", params: {} });
        
        // Notify Pyright about opened document
        const text = editor.getValue();
        window.LSPAPI.sendLSPMessage({
            jsonrpc: "2.0",
            method: "textDocument/didOpen",
            params: {
                textDocument: {
                    uri: "file:///untitled.py",
                    languageId: "python",
                    version: 1,
                    text,
                },
            },
        });
    });
    
    monaco.languages.registerCompletionItemProvider("python", {
        triggerCharacters: [".", " "],
        
        async provideCompletionItems(model, position) {
            const text = model.getValue();
            const line = position.lineNumber - 1;
            const character = position.column - 1;
            
            window.LSPAPI.sendLSPMessage({
                jsonrpc: "2.0",
                method: "textDocument/didChange",
                params: {
                    textDocument: {
                        uri: "file:///untitled.py",
                        version: 1,
                    },
                    contentChanges: [{ text }],
                },
            });
            
            const response = await sendRequest("textDocument/completion", {
                textDocument: { uri: "file:///untitled.py" },
                position: { line, character },
            });
            
            if (!response.result) return { suggestions: [] };
            
            const suggestions = response.result.items.map((item) => ({
                label: item.label,
                kind: monaco.languages.CompletionItemKind.Function,
                insertText: item.insertText || item.label,
                range: undefined,
            }));
            
            return { suggestions };
        },
    });
});
