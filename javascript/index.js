const dree = require('dree');
const path = require('path');
const fs = require('fs');
const utils = require('./utils');
const generators = require('./generators');

// Generate messages.json
const MESSAGES_HEADER_PATH = path.join(__dirname, '../', 'messages.h');
const messagesHeader = fs.readFileSync(MESSAGES_HEADER_PATH, 'utf-8');
const MESSAGES_PATH = path.join(__dirname, '../', 'messages.json');
const messages = (new utils.MessagesJsonGenerator(messagesHeader).generate());
fs.writeFileSync(MESSAGES_PATH, JSON.stringify(messages, null, 2));


// Get structure
const STRUCTURE_PATH = path.join(__dirname, '../', 'structure.json');
const structure = require(STRUCTURE_PATH);

// Parse every template
const ROOT_PATH = path.join(process.cwd());
const codes = utils.getCodes(generators, structure, messages);
dree.scan(ROOT_PATH, { exclude: [ /javascript/, /node_modules/ ] }, (file, _stat) => {
    if (file.name.indexOf('.template') !== -1) {
        const template = fs.readFileSync(file.path, 'utf-8');
        const generated = utils.parseTemplate(template, codes);
        const generatedPath = file.path.replace('.template', '');
        fs.writeFileSync(generatedPath, generated);
    }
});