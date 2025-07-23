const { contextBridge, ipcRenderer } = require("electron");

contextBridge.exposeInMainWorld("electronAPI", {
    selectWorkspace: () => ipcRenderer.invoke("select-workspace"),

    initEditor: () => ipcRenderer.invoke("init-editor"),
    onInitEditor: (callback) => ipcRenderer.on("init-editor", callback),
});

contextBridge.exposeInMainWorld("LSPAPI", {
    sendLSPMessage: (msg) => ipcRenderer.send("lsp-message", msg),
    onLSPMessage: (callback) => ipcRenderer.on("lsp-message", (_, message) => callback(message)),
});
