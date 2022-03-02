module.exports = {
    presets: [
      ["@babel/preset-env", { targets: { node: "current" } }],
      "@babel/preset-typescript"
    ],
    plugins : [
        '@babel/plugin-proposal-class-properties',
        '@babel/plugin-transform-react-constant-elements',
        '@babel/plugin-transform-runtime',
        '@babel/plugin-transform-regenerator'
      ],
      ignore : ['./diist', './build', './test', './node_modules']
  };