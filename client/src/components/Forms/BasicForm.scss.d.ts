declare namespace BasicFormScssNamespace {
  export interface IBasicFormScss {
    BasicForm: string;
  }
}

declare const BasicFormScssModule: BasicFormScssNamespace.IBasicFormScss & {
  /** WARNING: Only available when `css-loader` is used without `style-loader` or `mini-css-extract-plugin` */
  locals: BasicFormScssNamespace.IBasicFormScss;
};

export = BasicFormScssModule;
