/*
    RULES:
        THERE CANNOT BE ARRAYS DIRECTLY INSIDE OTHER ARRAYS
*/

class StructureDeallocatorCodeGenerator {

    _print(str) {
        this.result += `${str}\n`;
    }

    _getTypeName() {
        const elements = this.keys.map(({key, type}) => type === 'object' ? key : null).filter(val => val !== null);
        return (elements.length === 1 ? elements[0] : `${elements.slice(1).join('_')}_data`);
    }

    _getMaxCountName() {
        const elements = this.keys.map(({key, type}) => type === 'object' ? key : null).filter(val => val !== null);
        return `${elements.slice(1).join('_')}_max_count`;
    }

    _getCountName() {
        const elements = this.keys.map(({key, type}) => type === 'object' ? key : null).filter(val => val !== null);
        const last = elements.length - 1;
        return `${elements[last]}_count`;
    }

    _parseKey({ key, type }) {
        switch (type) {
            case 'array':
                return `[${key}]`;
            case 'object':
                return `.${key}`;
        }
    }

    _parseKeys() {
        return this.keys.slice(1).reduce((accumulator, current) => accumulator += this._parseKey(current), '').replace('.', '->');
    }

    _parseKeysParent(count) {
        const last = this.keys.length - 1;
        return `${this.keys.slice(1, last).reduce((accumulator, current) => accumulator += this._parseKey(current), '')}.${count}`.replace('.', '->');
    }

    _parse(data) {
        for (const key in data) {
            if (Array.isArray(data[key])) {
                this.keys.push(key);
                const keys = this._parseKeys();
                const count = this._getCountName();
                const keysParent = this._parseKeysParent(count);
                const type = this._getTypeName();
                const maxCount = this._getMaxCountName();
                this._print(`data${keys} = (${type}*)malloc(sizeof(${type}) * config->${maxCount});`);
                this._print(`data${keysParent} = 0;`);
                this._parse(data[key][0]);
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
        this.keys.push(key);
        this._parse(this.structure);
        return this.result;
    }

    constructor(structure) {
        this.structure = structure;
        this.keys = [];
        this.result = '';
    }

}

module.exports = StructureDeallocatorCodeGenerator;