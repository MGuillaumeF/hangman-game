declare global {
  class Error {
    constructor(message: string);
    constructor(message: string, options: {cause :any});
    stack?: string;
    message:string;
    name:string;
  }
}
