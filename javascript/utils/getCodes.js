function getCodes(generators, structure, messages) {
    return Object.keys(generators)
        .map(key => ({
            comment: generators[key].comment,
            code: (new generators[key].generator(structure, messages)).generate()
        }));
}

module.exports = getCodes;