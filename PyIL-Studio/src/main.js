const path = require("path");
const { app, BrowserWindow } = require("electron");

function createWindow() 
{
    const win = new BrowserWindow({
        width: 1200,
        height: 800,
        autoHideMenuBar: true,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
        }
    });
    
    win.loadFile(path.join(__dirname, "../dist/index.html"));
}

app.whenReady().then(createWindow);
