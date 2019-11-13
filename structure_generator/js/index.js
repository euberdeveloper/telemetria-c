const fs = require('fs');
const path = require('path');
const StructureCodeGenerator = require('./structureCodeGenerator');
const addInitialIndentation = require('./addInitilIndentation');

const COMMENT_NAME = '{{GENERATE_STRUCTURE_CODE}}';

const args = process.argv.slice(2);
const STRUCTURE_PATH = args[0];
const OUTPUT_NAME = args[1];
const CURRENT_DIR = process.cwd();

const structure = require(path.join(CURRENT_DIR, STRUCTURE_PATH));
const generator = new StructureCodeGenerator(structure);
const code = generator.generate();

const template = fs.readFileSync(path.join(__dirname, '../', 'template.h'), 'utf-8');
const output = template
                    .split('\n')
                    .map(row => (row.indexOf(COMMENT_NAME) === -1 ? row : addInitialIndentation(row, code)))
                    .join('\n');

fs.writeFileSync(path.join(__dirname, '../', OUTPUT_NAME), output);
