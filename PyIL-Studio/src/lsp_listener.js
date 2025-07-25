const { spawn } = require("child_process");
const { ipcMain, app } = require("electron");

module.exports = { listenOnLSPServer };

let lsProccess = spawn("pyright-langserver", ["--stdio"]);

function listenOnLSPServer(window) {
    let buff = "";
    lsProccess.stdout.on("data", (data) => {
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
            window.webContents.send("lsp-message", message);

            buff = buff.substring(totalLen);
        }
    });
}

ipcMain.on("lsp-message", (_, message) => {
    const json = JSON.stringify(message);
    const contentLen = Buffer.byteLength(json, "utf8");
    const payload = `Content-Length: ${contentLen}\r\n\r\n${json}`;
    lsProccess.stdin.write(payload);
});

app.on("will-quit", () => { if (lsProccess) lsProccess.kill(); });