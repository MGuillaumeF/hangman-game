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
  plugins: ["istanbul"],
  ignore: ["./dist", "./build", "./node_modules"]
};
