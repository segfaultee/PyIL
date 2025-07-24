const path = require("path");

const { app, BrowserWindow, ipcMain, dialog } = require("electron");
const { listenOnLSPServer } = require("./lsp_listener.js");

let window;

app.whenReady().then(() => {
    window = new BrowserWindow({
        width: 1200,
        height: 800,
        autoHideMenuBar: true,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
            preload: path.join(app.getAppPath(), "src/preload.js"),
        },
    });

    window.loadFile(path.join(app.getAppPath(), "index.html"));

    listenOnLSPServer(window);
});

ipcMain.handle("select-workspace", async () => {
    const result = await dialog.showOpenDialog({ properties: ["openDirectory"] });
    if (result.canceled) 
        return { canceled: true };
    return { canceled: false, path: result.filePaths[0] };
});

ipcMain.handle("init-editor", async () => {
    await window.loadFile(path.join(app.getAppPath(), "editor.html"));
    window.webContents.send("init-editor");
});
