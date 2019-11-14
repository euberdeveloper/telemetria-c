/*
    RULES:
        THERE CANNOT BE ARRAYS DIRECTLY INSIDE OTHER ARRAYS
*/

class ConfigInitializerCodeGenerator {

    _tabsFromIndentation() {
        let res = '';
        for (let i = 0; i < this.indentation; i++) {
            res += '\t';
        }
        return res;
    }

    _print(str) {
        this.result += `${this._tabsFromIndentation()}${str}\n`;
    }

    _getMaxCountName() {
        return `${this.keys.slice(1).join('_')}_max_count`;
    }

    _parse(data) {
        for (const key in data) {
            if (Array.isArray(data[key])) {
                this.keys.push(key);
                const maxCount = this._getMaxCountName();
                this._parse(data[key][0]);
                this._print(`else if (strcmp(keyString, "${maxCount}") == 0)`);
                this._print(`{`);
                this.indentation = 1;
                this._print(`toRtn->${maxCount} = atoi(valueString);`);
                this.indentation = 0;
                this._print(`}`);
                this.keys.pop();
            }
            else if (typeof data[key] === 'object') {
                this.keys.push(key);
                this._parse(data[key]);
                this.keys.pop();
            }
        }
    }

    generate() {
        this.keys.push({ key: 'data_t', type: 'object' });
        this._parse(this.structure);
        return this.result;
    }

    constructor(structure) {
        this.structure = structure;
        this.keys = [];
        this.indentation = 0;
        this.result = '';
    }

}

module.exports = ConfigInitializerCodeGenerator;