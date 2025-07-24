const path = require("path");

const MonacoWebpackPlugin = require("monaco-editor-webpack-plugin");
const CopyWebpackPlugin = require("copy-webpack-plugin");

module.exports = {
    mode: "production",
    target: "electron-renderer", 
    entry: "./src/renderer.js",
    output: {
        filename: "renderer.bundle.js",
        path: path.resolve(__dirname, "dist") + "/",
        publicPath: "",
        chunkFilename: "[name].[contenthash].js"
    },
    module: {
        rules: [
            {
                test: /\.css$/i,
                use: ["style-loader", "css-loader"],
            },
            {
                test: /\.ttf$/i,
                use: ["file-loader"],
            },
        ],
    },
    plugins: [
        new MonacoWebpackPlugin({ languages: ["python"] }),
        new CopyWebpackPlugin({ patterns: [
            { from: "package.json", to: "package.json" },
            { from: "src/lsp_listener.js", to: "src/lsp_listener.js" },
            { from: "src/main.js", to: "src/main.js" },
            { from: "src/preload.js", to: "src/preload.js" },
            { from: "src/renderer.js", to: "src/renderer.js" },
            
            { from: "src/html/editor.html", to: "editor.html" },
            { from: "src/html/index.html", to: "index.html" }
        ] }),
    ],
    resolve: { extensions: [".js", ".json"] },
    devtool: "source-map",
};
