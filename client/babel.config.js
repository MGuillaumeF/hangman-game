module.exports = {
  presets: [
    [
      "@babel/preset-env",
      {
        targets: { node: "current" },
        useBuiltIns: "entry",
        corejs: 3
      }
    ],
    "@babel/preset-typescript",
    "@babel/preset-react",
    ["react-app", { flow: false, typescript: true }]
  ],
  plugins: [
    "istanbul",
    ["@babel/plugin-proposal-class-properties", { loose: true }],
    ["@babel/plugin-proposal-private-methods", { loose: true }],
    "@babel/plugin-transform-react-constant-elements",
    "@babel/plugin-transform-runtime",
    "@babel/plugin-transform-regenerator"
  ],
  ignore: ["./dist", "./build", "./test", "./node_modules"]
};
