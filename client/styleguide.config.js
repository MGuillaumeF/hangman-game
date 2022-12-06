module.exports = {
  // eslint-disable-next-line @typescript-eslint/no-var-requires
  propsParser: require("react-docgen-typescript").withCustomConfig(
    "./tsconfig.json"
  ).parse,
  exampleMode: "expand",
  usageMode: "expand",
  styleguideDir: "dist/docs/components",
  pagePerSection: true,
  ignore: [
    "**/index.tsx",
    "**/__tests__/**",
    "**/*.test.{js,jsx,ts,tsx}",
    "**/*.spec.{js,jsx,ts,tsx}",
    "**/*.d.ts"
  ],
  theme: {
    color: {
      base: "#FFF",
      baseBackground: "#222",
      codebarBackground: "#333",
      link: "rgb(218, 133, 0);",
      linkHover: "rgb(238, 153, 0)",

      border: "#454545",
      sidebarBackground: "#303030",

      name: "#a48bf1",
      type: "#FEB73D",
      error: "#aa0000",

      // code area colors
      codeBase: "#A1B7D6",
      codeBackground: "#171717",
      codeComment: "#171717",
      codePunctuation: "#aa0000",
      codeProperty: "#a48bf1",
      codeDeleted: "#a48bf1",
      codeString: "#3DCEFF",
      codeInserted: "#690",
      codeOperator: "#aa0000",
      codeKeyword: "#a48bf1",
      codeFunction: "#FEB73D",
      codeVariable: "#3D76FF"
    }
  },
  sections: [
    {
      name: "Introduction",
      content: "README.md"
    },
    {
      name: "Documentation",
      pagePerSection: true,
      description: "The description for the documentation section",
      sections: [
        {
          name: "Components",
          pagePerSection: true,
          components: "src/components/**/*.tsx",
          exampleMode: "expand", // 'hide' | 'collapse' | 'expand'
          usageMode: "expand" // 'hide' | 'collapse' | 'expand'
        }
      ]
    },
    {
      name: "Live Demo",
      external: true,
      href: "https://github.com/MGuillaumeF/hangman-game#readme"
    }
  ]
};
