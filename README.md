# Doubly Linked Listig on Arrays (+ Easy Gay Grafics Graphviz)

A cache‑friendly list for real naturals who don’t like fucking default lists. For real naturals, because fuck in these lists is faster than in regular standard C++.

The God damn project has a built-in tool that shows the list status using **`Graphviz (dot)`**, spits out HTML logs, and highlights broken links automatically 0_0
---

## To note

* **(Sentinel node):** Null index of array (`0`) is a sentinel node:
  * `next[0]` point to **Tail** (the first node of array).
  * `prev[0]` point to **Head** (the last node of array).
* **A chain of free cells (`free`):** The empty cells are combined into a singly linked list within the same `next` array, which allows elements to be allocated and freed in $O(1)$ time.
* **Verificator:** Check connections with neighbors elements (`prev[next[i]] == i` и `next[prev[i]] == i`) and control array out.
* **(Graphviz + HTML (see you soon):** Generation of colored graphs (`SENTINEL`, `HEAD`, `TAIL`, `FREE`, `USED`, `ERROR`).

---

## The structer of project

```text
├── lists.h          
├── lists.cpp        
├── main.cpp        
├── dump.dot    
└── log.html     
