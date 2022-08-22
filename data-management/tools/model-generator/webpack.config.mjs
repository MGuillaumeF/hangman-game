/* eslint-disable no-undef */
/* eslint-disable @typescript-eslint/no-var-requires */

/**
 * Node imports
 */
import { unlinkSync } from "fs";
import { dirname, resolve } from "path";
import { fileURLToPath } from "url";
/**
 * algo compression
 */

/**
 * WebPack Plugins import
 */
import { CleanWebpackPlugin } from "clean-webpack-plugin";
import WebpackVisualizerPlugin from "webpack-visualizer-plugin2";

/**
 * Lint plugins
 * JS/TS
 */
import ESLintPlugin from "eslint-webpack-plugin";

/**
 * custom plugin
 */
import Logger from "../../../client/tools/webpack/Logger.js";
import PostBuildPlugin from "./tools/webpack/PostBuildPlugin.mjs";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const DEV = "development";
const PROD = "production";
const logger = Logger.getInstance();
logger.setLocation("./webpack.log");
logger.setLevel("INFO");

/**
 *
 * @param {string} mode
 * @param {string} level
 * @param {string} theme
 * @param  {...any} message
 */
function _trace(level, mode, theme, ...message) {
  if (message && message.length !== 0) {
    let webPackMode = "NONE";
    if (mode === DEV) {
      webPackMode = "DEV";
    } else if (mode === PROD) {
      webPackMode = "PROD";
    }

    if (typeof theme !== "string" || theme.length === 0 || theme.length > 25) {
      throw Error(
        `The theme "${theme}" is not supported by logger, please use string withe length [1, 25]`
      );
    }
    logger.info(
      `[${webPackMode}]`.padEnd(6, " "),
      theme.padEnd(25, " "),
      ...message
    );
  }
}

export default (env, args) => {
  /**
   * save webpack mode
   * development or production
   */
  const MODE = process.env.NODE_ENV;

  /**
   * Log wrapper to no set Mode or outputpath
   * @param {*} level
   * @param {*} theme
   * @param  {...any} message
   * @returns
   */
  const trace = (level, theme, ...message) =>
    _trace(MODE, level, theme, ...message);

  try {
    unlinkSync("./webpack.log");
    trace("INFO", "CLEAN", "Webpack log file cleaned...");
  } catch (exception) {
    trace("INFO", "CLEAN", "Webpack log file doesn't exist...");
  }
  trace("INFO", "INITIALISATION", "Webpack configuration run...");

  const config = {
    mode: MODE,
    entry: "./src/main.ts",
    target: "node",
    cache: false,
    output: {
      path: resolve(__dirname, "build"),
      publicPath: MODE === DEV ? "/" : process.env.PUBLIC_PATH || ".",
      filename: "[name].bundle.js"
    },
    resolve: {
      // Add `.ts` and `.tsx` as a resolvable extension.
      extensions: [
        ".ts",
        ".tsx",
        ".js",
        ".mjs",
        ".scss",
        ".svg",
        ".png",
        ".gif",
        ".jpg",
        ".jpeg"
      ]
    },
    module: {
      rules: [
        {
          test: /\.ts$/,
          use: [
            "babel-loader",
            {
              loader: "ts-loader"
            }
          ],
          exclude: [
            resolve(__dirname, "node_modules"),
            resolve(__dirname, "dist"),
            resolve(__dirname, "test")
          ]
        }
      ]
    },
    plugins: [
      new CleanWebpackPlugin(),
      new ESLintPlugin({
        emitError: true,
        emitWarning: true,
        failOnError: false,
        extensions: ["ts", "tsx", "js"],
        exclude: [
          "node_modules/**/*",
          "dist/**/*",
          "test/**/*",
          ".eslintrc.js",
          "webpack.config.mjs"
        ]
      }),
      new PostBuildPlugin()
    ]
  };
  /**
   * development part
   */
  if (MODE === DEV) {
    trace("INFO", "CONFIGURATION", "Load extra development configuration...");
    trace("INFO", "CONFIGURATION_DEBUG", "Add source map in development mode");
    config.devtool = "cheap-module-source-map";
    trace("INFO", "DEV_SERVER", "Define webpack server configuration");
  }
  /**
   * production part
   */
  if (MODE === PROD) {
    trace("INFO", "CONFIGURATION", "Load extra production configuration...");
    config.plugins.push(new WebpackVisualizerPlugin());
  }
  return config;
};
