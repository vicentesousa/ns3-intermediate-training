# Treinamento no ns-3 com aspectos complementares aos apresentados no tutorial

### Equipe
 - Idealizador/Orientador: Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br)
 - Coordenador: MSc. Daniel Luna (danielro@ufrn.edu.br)  
 - Equipe técnica: Daniel Luna (danielro@ufrn.edu.br), Daniel Flor (danielflor@ufrn.edu.br), Jéssika Cristina (jessie@ufrn.edu.br), Ravan Oliveira (ravan@ufrn.edu.br), Malco Dantas (malco@ufrn.edu.br), Lucas Ismael (lucasismael.gppcom@ufrn.edu.br) e Mateus Batista (mateus.gppcom@ufrn.edu.br).


## Fase 00: Preparação do ambiente de trabalho (prazo: 28/04/2019)

### [Hands-on 01: Github - o básico sobre repositórios](http://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_00/HD_01/f00_hd01.ipynb)
**Objetivos**
- Cadastramento no GitHub
- Criar repositorios no GitHub
- Submeter arquivos para repositórios do GitHub
- Editar o README.md

### [Hands-on 02: Básico do Jupyter Notebook](http://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_00/HD_02/f00_hd02.ipynb)
**Objetivos**
- Baixar, abrir, editar e criar Notebooks que executem blocos de código de Python (Python 3)
- Converter notebooks do Jupyter para PDF.

### [Hands-on 03: Instalando o ns-3 e integrando com o Eclipse](http://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_00/HD_03/f00_hd03.ipynb)
**Objetivos**
- Instalar o ns-3 e dependências;
- Integrar o ns-3 com o eclipse.


## Fase I: Desafios relacionados ao tutorial

### [Hands-on 01: Desafios relacionados ao exemplo first.cc](http://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_01/HD_01/f01_hd01.ipynb)
**Objetivos**
- Prática 01: envio de dois pacotes de tamanhos diferentes
- Prática 02: mudança na taxa de transmissão

**Equipe de execução:** Ravan Oliveira (ravan@ufrn.edu.br) e Malco Dantas (malco@ufrn.edu.br) - **Prazo:** 02/05/2019;

**Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) e Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br) - **Prazo:** 06/05/2019.


### [Hands-on 02: Desafios relacionados ao exemplo second.cc](http://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_01/HD_02/f01_hd02.ipynb)
**Objetivos**
- Prática 01: envio de dois pacotes de tamanhos diferentes sem criar novos objetos;
- Prática 02: aumentar número de nós via linha de comando;
- Prática 03: uso do tcpdump para insperção de arquivos pcap. 

**Equipe de execução:** Lucas Ismael (lucasismael.gppcom@ufrn.edu.br) e Mateus Batista (mateus.gppcom@ufrn.edu.br). - **Prazo:** 02/05/2019;

**Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) e Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br) - **Prazo:** 06/05/2019.


### [Hands-on 03: Desafios relacionados ao exemplo third.cc](http://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_01/HD_03/f01_hd03.ipynb)
**Objetivos**
- Prática 01: envio de três pacotes de STAs diferentes;
- Prática 02: uso do NetAnim para verificar o movimento dos usuários;
- Prática 03: uso do exemplo **rate-adaptation-distance.cc**. 

**Equipe de execução:** Jéssika Cristina (jessie@ufrn.edu.br) - **Prazo:** 02/05/2019;

**Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) e Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br) - **Prazo:** 06/05/2019.


### [Hands-on 4: Desafios relacionados ao exemplo third.cc parte 2](http://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_01/HD_04/f01_hd04.ipynb)
**Objetivos**
- Prática 01: Mudar modelo de mobilidade, plotar as posições das STAs com o GNUPlot.

**Equipe de execução:** Daniel Luna (danielro@ufrn.edu.br) - **Prazo:** 08/05/2019;

**Equipe de revisão:** Jéssika Cristina (jessie@ufrn.edu.br) - **Prazo:** 12/05/2019.


### [Hands-on 05: Desafios relacionados ao exemplo fifth.cc](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_01/HD_05/f01_hd05.ipynb)
**Objetivos**
- Prática 01: uso do GNUPlot para visualizar a janela de congestionamento.

**Equipe de execução:** Daniel Flor (danielflor@ufrn.edu.br) - **Prazo:** 02/05/2019;

**Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) e Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br) - **Prazo:** 06/05/2019.
<!---
comentário
-->
## Fase II: Campanhas de investigação com análise de desempenho do Wi-Fi

### [Hands-on 01: Campanha 0](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f01_hd01.ipynb)
**Objetivos**
 - Investigar o comportamento de diferentes camadas físicas do Wi-Fi com o algoritmos padrão de adaptação de taxa;
 - Definir a camada física que iremos adotar para os próximos experimentos;
 - Gerar um gráfico Throughput vs. Distância para cada padrão (uma linha para cada).

**Instruções para o hands-on**
 - Usar uma adaptação do rate-adaptation-distance.cc, feito para várias camadas PHY do Wi-Fi. Verificar se a taxa está sendo adaptada de maneira correta;
 - Gerar um gráfico Throughput vs. Distância (0, 50m, 100m, 150m, 200m e 250m).
 - Cada hands-on deve ter noção do Throughput de cada camada PHY do Wi-Fi (fazer um breve texto sobre cada camada PHY e quais funcionalidades estão implementadas no ns-3);
 - Só fazer para camadas PHY que tenham o algoritmo de Rate Adaptation implementado;
 - Explorar o máximo de configurações possíveis de cada camada PHY, por exemplo: channel bouning MIMO, tempo de guarda reduzido (TGI).

**Divisão de Tarefas**

**[Wifi 802.11a](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifia.ipynb)** - **Equipe de execução:** Ravan Oliveira (ravan@ufrn.edu.br) e Malco Dantas (malco@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wifi 802.11b](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifib.ipynb)**  - **Equipe de execução:** Ravan Oliveira (ravan@ufrn.edu.br) e Malco Dantas (malco@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wifi 802.11g](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifig.ipynb)** - **Equipe de execução:** Lucas Ismael (lucasismael.gppcom@ufrn.edu.br) e Mateus Batista (mateus.gppcom@ufrn.edu.br). - **Prazo:** 04/07/2019;

**[Wifi 802.11n](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifin.ipynb)**  - **Equipe de execução:** Lucas Ismael (lucasismael.gppcom@ufrn.edu.br) e Mateus Batista (mateus.gppcom@ufrn.edu.br). - **Prazo:** 04/07/2019;

**[Wifi 802.11ac](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifiac.ipynb)**  - **Equipe de execução:** Jéssika Cristina (jessie@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wifi 802.11ax](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifiax.ipynb)**  - **Equipe de execução:** Daniel Flor (danielflor@ufrn.edu.br) - **Prazo:** 04/07/2019;

**Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) e Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br) - **Prazo:** 08/07/2019;






