/* eslint-disable @typescript-eslint/no-var-requires */
const fs = require('fs');

const DefaultAppender = {
    type : 'FILE',
    // type : 'TCP'
    // type : 'HTTP'
    themes :  ['DEFAULT']
}


const availableLogLevels = ["DEBUG", "LOG", "INFO", "WARN", "ERROR"];

class Logger {
    static #instance = null;
    #level = 'INFO'
    #location = './template.log';
    #appenders = [];
    constructor(...appenders) {
        this.#appenders = appenders;
    }
    static getInstance = (...appenders) => {
        if (this.#instance === null) {
            this.#instance = new Logger(appenders);
        }
        return this.#instance;
    }
    trace = (level, theme, ...message) => {
        if (!availableLogLevels.includes(level)) {
            throw Error(
              `The level "${level}" is not supported by logger, please use one of them levels ${availableLogLevels.join(
                ", "
              )}`
            );
          }
        const now = new Date().toLocaleString();
        console[level.toLowerCase()](now, level, theme, ...message)
        fs.appendFileSync(
        this.#location,
         [now, level, theme, ...message, "\r\n"].join(";")
        );
    }
    setLevel = (level) => {
        this.#level = level;
    }
    setLocation = (location) => {
        this.#location = location;
    }
    debug = (theme, ...message) => {
        if (this.#level === 'DEBUG') {
            this.trace('DEBUG', theme, ...message);
        }
    }
    info = (theme, ...message) => {
        if (['DEBUG', 'INFO'].includes(this.#level)) {
            this.trace('INFO', theme, ...message);
        }
    }
    warn = (theme, ...message) => {
        if (this.#level !== 'ERROR') {
            this.trace('WARN', theme, ...message);
        }
    }
    error = (theme, ...message) => {
        this.trace('ERROR', theme, ...message);
    }
}

module.exports = Logger;