const path = require("path");

const MonacoWebpackPlugin = require("monaco-editor-webpack-plugin");
const CopyWebpackPlugin = require('copy-webpack-plugin');

module.exports = {
    mode: "development",
    target: "electron-renderer", 
    entry: "./src/renderer.js",
    output: {
        filename: "renderer.bundle.js",
        path: path.resolve(__dirname, "dist") + "/",
        publicPath: path.resolve(__dirname, "dist") + "/",
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
        new CopyWebpackPlugin({ patterns: [{ from: 'src/index.html', to: 'index.html' }] }),
    ],
    resolve: { extensions: [".js", ".json"] },
    devtool: "source-map",
};
