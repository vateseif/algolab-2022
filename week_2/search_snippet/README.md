# search snippets

## Problem

We are given a $n$ different words. Each word $i$ occurs $m_i$ times in a document at positions $p_{ij}$.

We are asked to find the smallest range $[a, b]$ that contains at least an instance of each of the $n$ words.

## Solution

We can store in a vector the total number of position $p_{ij}$ and sort it in increasing order. Then we can simply run sliding window and keep saving the window that stores all words with minimum range $[a, b]$.

We need a variable $n_{missing}$ that keeps count of the number of missing words in the interval $[a, b]$ and a vector `word_count` that stores how many instance of a word we have in the interval $[a, b]$

This can be done efficiently by updating $a, b$ as follows:

- Start with $n_{missing} = n, a=0, b=0$
- if $n_{missing} > 0:$
    - increase b by 1
    - increase `word_count` of new word
    - if count was 0 decrease $n_{missing}$
- while $n_{missing} = 0$:
    - increase a by 1
    - decrease `word_count` of word at previous a

## Complexity

Sliding window has complexity of $O(n)$. However we first need to sort the list so the total complexity is:

$$
O(N\log N)
$$

Where $N = \sum_i m_i$ is the total number of positions $p_{ij}$.