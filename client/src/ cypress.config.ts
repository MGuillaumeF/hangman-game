import { defineConfig } from "cypress";
import { devServer } from "@cypress/webpack-dev-server";
import webpackConfig from "./webpack.config";

export default defineConfig({
  component: {
    devServer(cypressConfig) {
      return devServer({
        framework: "react",
        cypressConfig,
        webpackConfig
      });
    }
  }
});
