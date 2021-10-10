AFSK Decoder
============

This is configurable command-line Audio Frequency Shift-Keying (AFSK) wav-file decoder. It extracts textual information from provided wav-files. It was made to solve Paradox Cat's coding challenge. 

Building
--------

To build this project, all you need is `CMake` and any reasonably modern `C++20` compiler.

1. Clone the repo.
2. `git submodule update --init --recursive`
3. `cmake .`
4. `make`

Test
----

To run the unit-tests, just do:

`./bin/afsk_decoder_test`


Usage
-----

To decode a file do:

`./bin/afsk_decoder_run -f <path to wav file>`

Fo example, you can try a provided sample data files:

`./bin/afsk_decoder_run -f data/file_3.wav`

Result:

```
messages:
=========

It was the White Rabbit, trotting slowly back again, and looking anxiously about as it went, as if it had lost something; and she heard it muttering to itself `The Duchess! The Duchess! Oh my dear paws! Oh my fur and whiskers! She'll get me executed, as sure as ferrets are ferrets! Where can I have dropped them, I wonder?' Alice guessed in a moment that it was looking for the fan and the pair of white kid gloves, and she very good-naturedly began hunting about for them, but they were nowhere to be seen--everything seemed to have changed since her swim in the pool, and the great hall, with the glass table and the little door, had vanished completely.
Very soon the Rabbit noticed Alice, as she went hunting about, and called out to her in an angry tone, `Why, Mary Ann, what are you doing out here? Run home this moment, and fetch me a pair of gloves and a fan! Quick, now!' And Alice was so much frightened that she ran off at once in the direction it pointed to, without trying to explain the mistake it had made.

`He took me for his housemaid,' she said to herself as she ran. `How surprised he'll be when he finds out who I am! But I'd better take him his fan and gloves--that is, if I can find them.' As she said this, she came upon a neat little house, on the door of which was a bright brass plate with the name `W. RABBIT' engraved upon it. She went in without knocking, and hurried upstairs, in great fear lest she should meet the real Mary Ann, and be turned out of the house before she had found the fan and gloves.

`How queer it seems,' Alice said to herself, `to be going messages for a rabbit! I suppose Dinah'll be sending me on messages next!' And she began fancying the sort of thing that would happen: `"Miss Alice! Come here directly, and get ready for your walk!" "Coming in a minute, nurse! But I've got to see that the mouse doesn't get out." Only I don't think,' Alice went on, `that they
```

Info Mode
---------

By default, the tool output outputs the encoded messages. You may want to get more information by providing `--info flag`:

`./bin/afsk_decoder_run -f data/file_3.wav --info`

This outputs information on the bits, bytes and message count and some insight on how many decoding erros it encountered:

Result:

```
parsing channel 1
detecting zero crossings
detected 66166 zero-crossings
detecting bits
detected 32360 bits
13234 zeros, 19126 ones
33807 errors
detecting bytes
detected 2784 bytes, 1 errors
decoded 64 messages, 5 errors
```

Raw Mode
--------

By adding `--raw` option to the command line, you will get raw bits and bytes output. This is useful for adjusting decoding parameters (see later):

`./bin/afsk_decoder_run -f data/file_3.wav --raw`

Result:

```
bits:
11111101011111100111111010111111111111110111100101011111111111111101111111111111111111111111111111110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111111101111111111011111111110111111111101111111111011111111110111111111101111111111011111111110111111111101111111111011111111 ...
```

```
bytes:
255 255 255 192 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 66 3 73 116 32 119 97 115 32 116 104 101 32 87 104 105 116 101 32 82 97 98 98 105 116 44 32 116 114 111 116 116 167 105 110 103 32 115 108 111 119 108 121 32 98 97 99 107 32 97 103 97 105 110 44 32 97 110 100 32 108 111 111 194 107 105 110 103 32 97 110 120 105 111 117 115 108 121 32 97 98 111 117 116 32 97 115 32 105 116 32 119 101 110 75 116 44 32 97 115 32 105 102 32 105 116 32 104 97 100 32 108 111 115 116 32 115 111 109 101 116 104 105 110 103 157 59 32 97 110 100 32 115 104 101 32 104 ...
```

Error Mode
----------

To get more info on decoding errors, add `--errors` command-line parameter. In the output you will see several types of errors:

1. Bit errors. The tool was unable to detect a bit from a segment duration between two zero-crossing:

`bit error: can't recognize segment length 90.702948 microseconds at segment: 66127 - 66128`

2. Byte errors. The tool failed to make a byte out of bit sequence.

```
byte error: stream ended before stop-bit at bit: 32360
bit context: 0110111111
```

3. Message errors. The tool failed to decode a message from a byte stream.

`message error: checksum error: expected 255, actual: 227 at byte: 2672`

Fine tuning
-----------

To adapt to noise and various conditions, the tool can be fine-tuned. There are three groups of arguments for that:

1. `--min-zero` and `--max-zero` set the range for zero-byte detection in microseconds. Defaults to `600-680` microseconds

For example, to allow the tool to detect zero bits in `500-900` range, use:

`./bin/afsk_decoder_run -f data/file_3.wav --min-zero 500 --max-zero 900`

2. `--min-one` and `--max-one`. The same for one bit, see previous paragraph.

3. In order to detect zero-crossings, the tool uses a moving window of `N` samples and applies the algorithm on it. The default window size is 3 samples, but you can adjust it with `--zc-resolution` parameter:

`./bin/afsk_decoder_run -f data/file_3.wav --zc-resolution 4`
