Este repositório contém códigos e dados coletados durante o desenvolvimento do projeto de iniciação ciêntifica (PIBIC) intitulado como: *Análise comparativa de desempenho entre algoritmos paralelos e tradicionais para processamento de imagens.*

**Um breve resumo sobre o projeto:**

Neste projeto, realizamos uma análise comparativa entre algoritmos paralelos e tradicionais no processamento de imagens, focando em tempo de processamento, eficiência no uso de recursos e qualidade dos resultados. A pesquisa envolve experimentos práticos com transformações radiométricas, detecção de bordas e filtragem espacial em diferentes conjuntos de imagens. Além de avaliar o desempenho, o estudo explora a complexidade de implementação e as aplicações práticas da programação paralela, oferecendo uma compreensão clara de seu potencial para melhorar a eficiência e eficácia no processamento de grandes volumes de dados de imagens.

**A linguagem utilizada para aproveitar os recursos da programação paralela foi a Halide, que pode ter sua documentação consultada pelo link abaixo:**

* https://halide-lang.org/
* https://github.com/halide/Halide

**As linguagens utilizadas para testar as versões tradicionais foram Python e C:**

* https://www.python.org/
* https://visualstudio.microsoft.com
* https://embarcados.com.br/linguagem-c-guia-completo/

**O projeto é dividido em alguns diretórios especificos:**

1. **C_Codigos** - contém os códigos desenvolvidos utilizando a linguagem C.
2. **Halide_Codigos** - contém os códigos desenvolvidos utilizando a linguagem Halide.
3. **Python_Codigos** - contém os códigos desenvolvidos utilizando a linguagem Python.
4. **Imagens_Selecionadas** - contém as imagens selecionadas para realizar os testes dos algoritmos.
5. **Metricas** - contém os códigos desenvolvidos utilizando a linguagem Python para observar o ganho de desempenho entre as versões Paralela e Tradicional.
6. **Docs** - contém o relatório final do projeto, tabelas comparativas contendo ganho de desempenho e dados de tempos coletados utilizados para análise de ganho de desempenho.

**Para clonar o repostirório e utilizar na máquina local utilize o comando abaixo:**

* git clone https://github.com/SebasNeto/Processamento-de-Imagens---Programacao-Paralela.git
