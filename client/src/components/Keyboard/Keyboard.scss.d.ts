declare namespace KeyboardScssNamespace {
  export interface IKeyboardScss {
    Keyboard: string;
  }
}

declare const KeyboardScssModule: KeyboardScssNamespace.IKeyboardScss & {
  /** WARNING: Only available when `css-loader` is used without `style-loader` or `mini-css-extract-plugin` */
  locals: KeyboardScssNamespace.IKeyboardScss;
};

export = KeyboardScssModule;
