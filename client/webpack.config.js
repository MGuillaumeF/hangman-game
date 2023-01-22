/* eslint-disable no-undef */
/* eslint-disable @typescript-eslint/no-var-requires */

/**
 * Node imports
 */
const path = require("path");
const fs = require("fs");

/**
 * algo compression
 */
const zopfli = require("@gfx/zopfli");

/**
 * WebPack Plugins import
 */
const HtmlWebpackPlugin = require("html-webpack-plugin");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const CssMinimizerPlugin = require("css-minimizer-webpack-plugin");
const { CleanWebpackPlugin } = require("clean-webpack-plugin");
const CompressionPlugin = require("compression-webpack-plugin");
const WebpackVisualizerPlugin = require("webpack-visualizer-plugin2");

/**
 * Lint plugins
 * JS/TS
 * CSS/SCSS/SASS
 */
const ESLintPlugin = require("eslint-webpack-plugin");
const StylelintPlugin = require("stylelint-webpack-plugin");

/**
 * custom plugin
 */
const PreBuildPlugin = require("./tools/webpack/PreBuildPlugin");
const PostBuildPlugin = require("./tools/webpack/PostBuildPlugin");
const Logger = require("./tools/webpack/Logger");

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

module.exports = (env, args) => {
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
    fs.unlinkSync("./webpack.log");
    trace("INFO", "CLEAN", "Webpack log file cleaned...");
  } catch (exception) {
    trace("INFO", "CLEAN", "Webpack log file doesn't exist...");
  }
  trace("INFO", "INITIALISATION", "Webpack configuration run...");

  const config = {
    mode: MODE,
    entry: {
      main: "./src/index.tsx",
      book: "./src/book/index.tsx"
    },
    cache: false,
    output: {
      path: path.resolve(__dirname, "build"),
      publicPath: MODE === DEV ? "/" : process.env.PUBLIC_PATH || ".",
      filename:
        MODE === DEV ? "[name].bundle.js" : "[name].[contenthash].bundle.js"
    },
    resolve: {
      // Add `.ts` and `.tsx` as a resolvable extension.
      extensions: [
        ".ts",
        ".tsx",
        ".js",
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
          test: /\.s[ac]ss$/i,
          use: [
            {
              loader: "@teamsupercell/typings-for-css-modules-loader",
              options: {
                formatter: "prettier"
              }
            },
            // Translates CSS into CommonJS
            {
              loader: "css-loader",
              options: {
                modules: true
              }
            },
            // prefix for css rules
            {
              loader: "postcss-loader",
              options: {
                postcssOptions: {
                  plugins: [
                    [
                      "autoprefixer",
                      {
                        // Options
                      }
                    ]
                  ]
                }
              }
            },
            // Compiles Sass to CSS
            "sass-loader"
          ]
        },
        {
          test: /\.tsx?$/,
          use: [
            "babel-loader",
            {
              loader: "ts-loader"
            }
          ],
          exclude: [
            path.resolve(__dirname, "node_modules"),
            path.resolve(__dirname, "dist"),
            path.resolve(__dirname, "test")
          ]
        },
        {
          test: /\.(png|jpe?g|gif|svg)$/i,
          use: [
            // {
            //   loader: "babel-loader"
            // },
            {
              loader: "url-loader",
              options: {
                limit: 8192
              }
            }
            // {
            //   loader: "file-loader",
            //   options: {
            //     name: "[contenthash].[ext]",
            //     outputPath: path.resolve(__dirname, "dist")
            //   }
            // }
          ]
        }
      ]
    },
    plugins: [
      new PreBuildPlugin(),
      new HtmlWebpackPlugin({
        inject: "head",
        title: "Hangman Game",
        template: path.resolve(__dirname, "public/index.html"),
        favicon: path.resolve(__dirname, "public/favicon.ico"),
        chunks: ["main"],
        filename: "index.html"
      }),
      new HtmlWebpackPlugin({
        inject: "head",
        title: "Hangman Game - Story Book",
        template: path.resolve(__dirname, "public/index.html"),
        favicon: path.resolve(__dirname, "public/favicon.ico"),
        chunks: ["book"],
        filename: "book/index.html"
      }),
      // new CleanWebpackPlugin(),
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
          "webpack.config.js"
        ]
      }),
      new StylelintPlugin({
        exclude: ["node_modules/**/*", "dist/**/*", "test/**/*"],
        extensions: ["scss"],
        customSyntax: "postcss-scss"
      }),
      new PostBuildPlugin()
    ]
  };
  /**
   * development part
   */
  if (MODE === DEV) {
    trace("INFO", "CONFIGURATION", "Load extra development configuration...");
    trace(
      "INFO",
      "CONFIGURATION_DEBUG",
      "Add dirty/fast CSS bundle and import as style tag"
    );
    config.module.rules[0].use.unshift(
      // Creates `style` nodes from JS strings
      "style-loader"
    );
    trace("INFO", "CONFIGURATION_DEBUG", "Add source map in development mode");
    config.devtool = "cheap-module-source-map";
    trace("INFO", "DEV_SERVER", "Define webpack server configuration");
    config.devServer = {
      historyApiFallback: true,
      client: {
        overlay: {
          errors: true,
          warnings: false
        }
      },
      //static: [path.join(__dirname, "dist")],
      //compress: true,
      port: 3000,
      hot: true
    };
  }
  /**
   * production part
   */
  if (MODE === PROD) {
    trace("INFO", "CONFIGURATION", "Load extra production configuration...");
    trace(
      "INFO",
      "CONFIGURATION_PERFO",
      "Remove dirty/fast CSS bundle and import as style tag"
    );
    config.module.rules[0].use.unshift(
      // Extract CSS as file
      MiniCssExtractPlugin.loader
    );
    trace("INFO", "CONFIGURATION_DEBUG", "Add source map in production mode");
    trace("INFO", "CONFIGURATION_PERFO", "Add optimizations");
    config.optimization = {
      // [...]
      emitOnErrors: true,
      mergeDuplicateChunks: true,
      minimize: true,
      minimizer: [new CssMinimizerPlugin()],
      usedExports: true,
      removeAvailableModules: true,
      splitChunks: {
        chunks: "all"
      }
    };
    trace(
      "INFO",
      "CONFIGURATION_PERFO",
      "Add CSS bundle compactor and importer"
    );
    config.plugins.push(
      new CssMinimizerPlugin(),
      new MiniCssExtractPlugin(
        {
          filename: "[name].css",
          chunkFilename: "[id].css"
        }
        //new PostBuildPlugin()
      ),
      new CompressionPlugin({
        compressionOptions: {
          numiterations: 15
        },
        algorithm(input, compressionOptions, callback) {
          return zopfli.gzip(input, compressionOptions, callback);
        }
      }),
      new WebpackVisualizerPlugin()
    );
  }
  return config;
};
