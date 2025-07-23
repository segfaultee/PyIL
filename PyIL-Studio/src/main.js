const path = require("path");
const { app, BrowserWindow, ipcMain, dialog } = require("electron");
const { spawn } = require("child_process");

let mainWindow;
let pyrightProc;

function createWindow() {
    mainWindow = new BrowserWindow({
        width: 1200,
        height: 800,
        autoHideMenuBar: true,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
            preload: path.join(app.getAppPath(), "src/preload.js"),
        },
    });

    mainWindow.loadFile(path.join(app.getAppPath(), "index.html"));
}

app.whenReady().then(() => {
    createWindow();

    pyrightProc = spawn("pyright-langserver", ["--stdio"]);

    let buff = "";
    pyrightProc.stdout.on("data", (data) => {
        buff += data.toString();

        while (true) {
            const endIndex = buff.indexOf("\r\n\r\n");
            if (endIndex === -1) break;

            const header = buff.substring(0, endIndex);
            const match = header.match(/Content-Length: (\d+)/);

            if (!match) break;

            const length = parseInt(match[1]);
            const totalLen = endIndex + 4 + length;

            if (buff.length < totalLen) break;

            const message = JSON.parse(buff.substring(endIndex + 4, totalLen));
            mainWindow.webContents.send("lsp-message", message);

            buff = buff.substring(totalLen);
        }
    });

    ipcMain.on("lsp-message", (_, message) => {
        const json = JSON.stringify(message);
        const contentLen = Buffer.byteLength(json, "utf8");
        const payload = `Content-Length: ${contentLen}\r\n\r\n${json}`;
        pyrightProc.stdin.write(payload);
    });

    app.on("will-quit", () => {
        if (pyrightProc) pyrightProc.kill();
    });
});

ipcMain.handle("select-workspace", async () => {
    const result = await dialog.showOpenDialog({
        properties: ["openDirectory"]
    });
    if (result.canceled) return { canceled: true };

    global.workspace = result.filePaths[0];
    return { canceled: false, path: global.workspace };
});

ipcMain.handle("init-editor", async () => {
    if (!mainWindow) return;

    await mainWindow.loadFile(path.join(app.getAppPath(), "editor.html"));

    mainWindow.webContents.send("init-editor");
});
