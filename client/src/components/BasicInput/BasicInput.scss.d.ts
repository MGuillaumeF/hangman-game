declare namespace BasicInputScssNamespace {
  export interface IBasicInputScss {
    BasicInput: string;
  }
}

declare const BasicInputScssModule: BasicInputScssNamespace.IBasicInputScss & {
  /** WARNING: Only available when `css-loader` is used without `style-loader` or `mini-css-extract-plugin` */
  locals: BasicInputScssNamespace.IBasicInputScss;
};

export = BasicInputScssModule;
