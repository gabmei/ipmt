# Projeto 2 de IF767
Ferramenta de busca offline e compressão/descompressão de arquivos
##### Dupla
- Daniel Pacheco
- Gabriel Meireles

##### Instruções de compilação

Na pasta raiz do projeto execute o comando make para gerar o executavel na pasta ./bin

```
    ../pmt$ make 
```

##### Instruções de uso
```
./bin/ipmt index textfile                         index textfile to a .idx file
./bin/ipmt search [options] pattern indexfile     search for pattern in indexfile
options:
- c            :                                  count occorrences of pattern(s) in indexfile
- p patternfile:                                  search for multiple patterns in indexfile
./bin/ipmt zip textfile                           compress textfile to textfile.myz with Huffman's algorithm
./bin/ipmt unzip textfile.myz                     decompress textfile.myz to textfile with Huffman's algorithm
```
