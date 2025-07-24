const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("workspaceAPI", {
    selectWorkspace: () => ipcRenderer.invoke("select-workspace"),
    getWorkspace: () => ipcRenderer.invoke("get-workspace"),
    onSelectWorkspace: (callback) => ipcRenderer.on("select-workspace", callback),
});

contextBridge.exposeInMainWorld("editorAPI", {
    initEditor: () => ipcRenderer.invoke("init-editor"),
    onInitEditor: (callback) => ipcRenderer.on("init-editor", callback),
});

contextBridge.exposeInMainWorld("LSPAPI", {
    sendLSPMessage: (msg) => ipcRenderer.send("lsp-message", msg),
    onLSPMessage: (callback) => ipcRenderer.on("lsp-message", (_, message) => callback(message)),
});
