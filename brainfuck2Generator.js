class BrainFuck2Generator {

    generate() {
        this.rows = this.header
            .split('\n')
            .filter(row => (row.split(' '))[0] === '#define' && (row.split(' ').length > 3));
        this.rows.forEach(row => {
            const message = {};
            const words = row.split(' ');
            const [_def, name, id] = words;
            message.name = name;
            message.id = id;
            const bytesIndex = words.indexOf('-->');
            const bytes = words.slice(bytesIndex + 1).filter(el => el !== '|').filter((_, i) => i % 2 !== 0);
            for (let i = 0; i < bytes.length; i++) {
                let value = bytes[i];
                value = value.split('/')[0];
                value = value.split('%')[0];
                message[`byte${i}`] = value;
            }
            this.result.push(message);
        });
        return this.result;
    }


    constructor(header) {
        this.header = header;
        this.result = [];
    }

}

const fs = require('fs');
const header = fs.readFileSync('./example.h', 'utf-8');
const generator = new BrainFuck2Generator(header);
console.log(JSON.stringify(generator.generate(), null, 2));