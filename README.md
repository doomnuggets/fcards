# fcards

This tool allows you to learn anything you want in the form of flash-cards.


## How do I get started?


Clone the repo and build the binary. (Read the code fist if you dare - I'm terrible at `C`).

```
git clone xyz
cd xyz && make
```

To add your own cards you can create a directory inside the `path/to/repo/decks/` directory.
Inside this directory you can create your files (cards).

A simple example could look like this:

```
mkdir path/to/repo/decks/my-cards
vim path/to/repo/decks/my-cards/001_card
```

The format of the cards is pretty simple. You separate the question from the
answer with a `\n-xxx-\n` marker, this way the tool should be able to parse
your card correctly.


An example card may look like this:

```
What is the answer to life, the universe and everything?

-xxx-

42
```

To ensure your cards are in the order you'd like, you have to prefix the card files with numbers.
For instance:

```
decks
└── test
    ├── 001_card
    ├── 002_card
    ├── 003_card
    ├── 004_overflow
    └── 005_format_str
```


## License

Refer to the [LICENSE](LICENSE) file if you're into that :)
