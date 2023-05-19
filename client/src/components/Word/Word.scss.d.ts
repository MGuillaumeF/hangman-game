declare namespace WordScssNamespace {
  export interface IWordScss {
    Word: string;
    empty: string;
    letter: string;
  }
}

declare const WordScssModule: WordScssNamespace.IWordScss & {
  /** WARNING: Only available when `css-loader` is used without `style-loader` or `mini-css-extract-plugin` */
  locals: WordScssNamespace.IWordScss;
};

export = WordScssModule;
