module.exports = {
  root: true,
  env: {
    node: true
  },
  parser: "@typescript-eslint/parser",
  parserOptions: {
    ecmaVersion: 2020,
    sourceType: "module"
  },
  extends: [
    "prettier",
    "eslint:recommended",
    "plugin:@typescript-eslint/recommended",
    "plugin:promise/recommended",
    "plugin:typescript-sort-keys/recommended",
    "plugin:node/recommended"
  ],
  plugins: [
    "@typescript-eslint",
    "typescript-sort-keys",
    "prettier",
    "promise"
  ],
  rules: {
    "node/no-unpublished-import": "off",
    "node/no-unsupported-features/es-syntax": "off",
    "node/no-missing-import": "off"
  }
};
