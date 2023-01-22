declare namespace StyleScssNamespace {
  export interface IStyleScss {
    SettingsContainer: string;
    details: string;
    fadeEnter: string;
    fadeEnterActive: string;
    fadeEnterDone: string;
    fadeExit: string;
    fadeExitActive: string;
    fadeExitDone: string;
    "hover-summary": string;
    item: string;
    menus: string;
    summary: string;
  }
}

declare const StyleScssModule: StyleScssNamespace.IStyleScss & {
  /** WARNING: Only available when `css-loader` is used without `style-loader` or `mini-css-extract-plugin` */
  locals: StyleScssNamespace.IStyleScss;
};

export = StyleScssModule;
