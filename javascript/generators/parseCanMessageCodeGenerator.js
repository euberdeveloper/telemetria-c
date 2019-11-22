class ParseCanMessageCodeGenerator {

    _tabsFromIndentation() {
        let res = '';
        for (let i = 0; i < this.indentation; i++) {
            res += '\t';
        }
        return res;
    }

    _parseKeys() {
        return this.keys.reduce((accumulator, current) => accumulator += `.${current}`, '').replace('.', '->');
    }

    _getCountName() {
        const last = this.keys.length - 1;
        return `${this.keys[last]}_count`;
    }

    _print(str) {
        this.result += `${this._tabsFromIndentation()}${str}\n`;
    }

    _parseValue(key, value, message) {
        const [type, name] = value.split('|');
        const keyBase = 'byte';
        let start = null, end = null, expr = '';
        for (let i = 0; i < 8; i++) {
            if (message[`${keyBase}${i}`] === name) {
                end = i;
                start = (start === null) ? i : start;
            }
        }
        if (start === null) {
            console.warn(`Warning: not found ${name} in message ${message.name}`);
            return;
        }
        for (let i = end, j = 0; i >= start; i--) {
            expr += `+ (${keyBase}[${i}] * ${16 ** (j++)})`;
        }
        expr = expr.replace('+ ', '');
        this._print(`data${this._parseKeys()}[data${this._parseKeys()}_count].${key} = ${expr};`);
    }

    _parseMessage(object, maxCount, name) {
        const message = this.messages.find(m => m.name === name);
        if (message) {
            this._print(`case ${message.id}:`);
            this.indentation++;
            this._print(`if (data${this._parseKeys()}_count < ${maxCount})`);
            this._print(`{`);
            this.indentation++;

            this._print(`data${this._parseKeys()}[data${this._parseKeys()}_count].timestamp = timestamp;`);
            if (typeof object.value === 'object') {
                for (const key in object.value) {
                    this._parseValue(`value.${key}`, object.value[key], message);
                }
            }
            else {
                this._parseValue('value', object.value, message);
            }
            this._print(`data${this._parseKeys()}_count++;`);

            this.indentation--;
            this._print(`}`);
            this._print(`break;`);
            this.indentation--;
        }
        else {
            console.warn(`Warning: not found a message with name ${name}`);
        }
    }

    _parseArray(data) {
        const object = data[0];
        const maxCount = data[1];
        const name = data[2];
        this._parseMessage(object, maxCount, name);
    }

    _parseObject(data) {
        ;
        for (const key in data) {
            this.keys.push(key);
            this._parse(data[key], key);
            this.keys.pop();
        }
    }

    _parse(data) {
        if (Array.isArray(data)) {
            this._parseArray(data);
        }
        else if (typeof data === 'object') {
            this._parseObject(data);
        }
        // else {
        //     this._parsePrimitive(data);
        // }
    }

    _firstParse(data) {
        for (const key in data) {
            this.keys.push(key);
            this._parse(data[key]);
            this.keys.pop();
        }
    }

    generate() {
        this._print(`switch(id)`);
        this._print(`{`);
        this.indentation++;
        this._firstParse(this.structure);
        this.indentation--;
        this._print(`}`);
        return this.result;
    }

    constructor(structure, messages) {
        this.structure = structure;
        this.messages = messages;
        this.indentation = 0;
        this.keys = [];
        this.result = '';
    }

}

module.exports = ParseCanMessageCodeGenerator;