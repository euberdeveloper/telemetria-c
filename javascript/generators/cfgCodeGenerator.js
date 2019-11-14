/*
    RULES:
        THERE CANNOT BE ARRAYS DIRECTLY INSIDE OTHER ARRAYS
*/

class CfgCodeGenerator {

    _print(str) {
        this.result += `${str}\n`;
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
                this._print(`"${maxCount}": ${data[key][1]},`);
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
        const lastIndex = this.result.lastIndexOf(',');
        return this.result.slice(0, lastIndex);
    }

    constructor(structure) {
        this.structure = structure;
        this.keys = [];
        this.result = '';
    }

}

module.exports = CfgCodeGenerator;