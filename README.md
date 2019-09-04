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

**Equipe de execução:** Jéssika Cristina (jessie@ufrn.edu.br) - **Prazo:** 02/05/2019;

**Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) e Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br) - **Prazo:** 06/05/2019.


### [Hands-on 04: Desafios relacionados ao exemplo third.cc parte 2](http://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_01/HD_04/f01_hd04.ipynb)
**Objetivos**
- Prática 01: Mudar modelo de mobilidade, plotar as posições das STAs com o GNUPlot.

**Equipe de execução:** Daniel Luna (danielro@ufrn.edu.br) - **Prazo:** 08/05/2019;

**Equipe de revisão:** Jéssika Cristina (jessie@ufrn.edu.br) - **Prazo:** 12/05/2019.


### [Hands-on 05: Desafios relacionados ao exemplo fifth.cc](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_01/HD_05/f01_hd05.ipynb)
**Objetivos**
- Prática 01: uso do GNUPlot para visualizar a janela de congestionamento.

**Equipe de execução:** Daniel Flor (danielflor@ufrn.edu.br) - **Prazo:** 02/05/2019;

**Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) e Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br) - **Prazo:** 06/05/2019.

### [Hands-on 06: Uso do exemplo rate-adaptation-distance.cc](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_01/HD_06/f01_hd06.ipynb)
**Objetivos**
- Descrever o exemplo rate-adaptation-distance.cc.
- Gerar o gráfico Throughput vs. Distância para os valores padrões do script.

**Equipe de execução:** Jéssika (jessie@ufrn.edu.br);

**Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) e Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br).

<!---
comentário
-->
## Fase II: Campanhas de investigação com análise de desempenho do Wi-Fi

### [Hands-on 01: Campanha 0](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01.ipynb)
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

**[Wi-Fi 802.11a](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifia.ipynb)** - **Equipe de execução:** Malco Dantas (malco@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wi-Fi 802.11b](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifib.ipynb)**  - **Equipe de execução:** Ravan Oliveira (ravan@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wi-Fi 802.11g](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifig.ipynb)** - **Equipe de execução:** Lucas Ismael (lucasismael.gppcom@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wi-Fi 802.11n](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifin.ipynb)**  - **Equipe de execução:** Lucas Ismael (lucasismael.gppcom@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wi-Fi 802.11ac](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifiac.ipynb)**  - **Equipe de execução:** Jéssika Cristina (jessie@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wi-Fi 802.11ax](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01/f02_hd01_wifiax.ipynb)**  - **Equipe de execução:** Daniel Flor (danielflor@ufrn.edu.br) - **Prazo:** 04/07/2019;

**Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) e Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br) - **Prazo:** 08/07/2019;


### [Hands-on 1.5: FlowMonitor, campanhas e plotagem de gráficos](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_01-5/f02_hd01-5.ipynb)
**Objetivos**
 - Usar o FlowMonitor no ns-3;
 - Campanhas de Simulação;
 - Plotagem de Gráficos usando o Matlab (Licença Educacional LabSim/DCO - UFRN).

**Instruções para o hands-on**
 - Descrever a inserção do FlowMonitor em um script do ns-3 (pode ser do Wi-Fi).
 - Criação das campanhas de simulação (um *.sh*) por meio de um script do Matlab (Licença Educacional LabSim/DCO - UFRN);
 - Simular os diversos cenários, variando os parâmetros necessários para um número definido de Jobs;
 - Plotar os gráficos utilizando o Matlab (Licença Educacional LabSim/DCO - UFRN).

 - **Equipe de execução:** Daniel Luna (danielro@ufrn.edu.br) - **Prazo:** 11/07/2019;

 **Equipe de revisão:** Daniel Flor (danielflor@ufrn.edu.br) e Jéssika Cristina (jessie@ufrn.edu.br)  - **Prazo:** 15/07/2019;

### [Hands-on 02: Campanha 1](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_02/f02_hd02.ipynb)
**Objetivos**
 - Analisar o comportamento do sistema com o aumento do **tráfego oferecido**;
 - Analisar o comportamento do sistema com o aumento da **duração da simulação**;
 - Analisar o comportamento do sistema com o aumento do **número de Jobs da simulação**;

**Instruções para o hands-on**
 - Utilizar os scripts de criação das campanhas da Fase 02 - Hands-on 1.5;
 - Fazer simulações variando:
 	- Taxa oferecida: 500kpbs, 1, 10, 50, 100, 200 Mbps;
 	- Número de STAs: 10;
	- Tempo de simulação: 4, 25 e 50 segundos;
	- Simulation runs: 10, 50, 100, 500 e 1000 (só precisa rodar o 1000);
	- STAs parados e posicionados aleatoriamente;
	- Wi-Fi infraestruturado com 1 AP no centro de um grid retangular de 100x100m.
 - Plots:
	- (i) Throughput médio do AP vs taxa oferecida para cada SimTime (3 curvas); 
	- (ii) PLR médio do AP vs taxa oferecida para cada SimTime (3 curvas);
	- (iii) Throughput médio dos STAs vs taxa oferecida para cada SimTime (3 curvas); 
	- (iv) PLR médio dos STAs vs taxa oferecida para cada SimTime (3 curvas);
	- (v) Repetir o plot (i), mas fazer com um SimTime específico para vários valores de Jobs. 
 - Análise deve ser feita apenas para **Downlink** e **Uplink**.

 - **Equipe de execução:** Daniel Flor (danielflor@ufrn.edu.br) e Daniel Luna (danielro@ufrn.edu.br)- **Prazo:** 22/07/2019.

- **Equipe de revisão:** Jéssika Cristina (jessie@ufrn.edu.br) - **Prazo:** 24/07/2019.

### [Hands-on 03: Campanha 2](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_03/f02_hd03.ipynb)
**Objetivos**
 - Analisar o comportamento do sistema com o aumento do número de STAs (tráfego oferecido constante).

**Instruções para o hands-on**
 - Utilizar os scripts de criação das campanhas da Fase 02 - Hands-on 1.5;
 - Fazer simulações variando:
  - Taxa oferecida: 1.5Mbps (downlink) e 750kbps (uplink);
  - Número de STAs: 1, 2, 3, 4, 5, 6, 7, 8, 9 e 10;
  - Tempo de simulação: tempo de simulação e Jobs definidos pela campanha 1;
  - STAs parados e posicionados aleatoriamente.
 - Plots:
	- (i) Throughput vs # de STAs;; 
	- (ii) PLR vs # de STAs;;
	- (iii) User satisfaction vs # de STAs. 
 - Análise deve ser feita para **Downlink** e **Uplink**.

 - **Equipe de execução:** Jéssika Cristina (jessie@ufrn.edu.br) e Daniel Luna (danielro@ufrn.edu.br)- **Prazo:** 22/07/2019.

- **Equipe de revisão:** Daniel Flor - **Prazo:** 24/07/2019.

### [Hands-on 04: Campanha 3](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/fase_02/HD_04/f02_hd04.ipynb)
**Objetivos**
 - Analisar o comportamento de diferentes algoritimos de adaptação de taxa com o aumento do número de STAs (tráfego oferecido constante).

**Instruções para o hands-on**
 - Utilizar os scripts de criação das campanhas da Fase 02 - Hands-on 1.5;
 - Fazer simulações variando:
 	- Taxa oferecida: 1.5Mbps (downlink) e 750kbps (uplink);
 	- Número de STAs: 1, 2, 3, 4, 5, 6, 7, 8, 9 e 10;
	- Tempo de simulação: 500 segundos;
	- Simulation runs: 1000;
	- STAs parados e posicionados aleatoriamente;
	- Repetir os estudos do TCC do Guilherme.
 - Plots:
 	- (i) Tput vs # de STAs; 
 	- (ii) PLR vs # de STAs; 
 	- (iii) user satisfaction vs # de STAs;
 - Análise deve ser feita para **Downlink** e **Uplink**.

 - **Equipe de execução:** Lucas Ismael (lucasismael.gppcom@ufrn.edu.br), Malco Dantas (malco@ufrn.edu.br) e Ravan Oliveira (ravan@ufrn.edu.br)  - **Prazo:** 22/07/2019.

- **Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br)- **Prazo:** 24/07/2019.

## Ferramentas Complementares

Série de Hands-on que descreve as ferramentas, problemas e soluções encontrados ao longo do desenvolvimento do treinamento ns-3-intermediate-training.

### [Hands-on 1: Uso do Núcleo de Processamento de Alto Desempenho - NPAD (UFRN)](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/ferramentas_complementares/HD_01/fc_hd01.ipynb)

**Objetivos**
 - Simulando no supercomputador (NPAD) da UFRN.
 
**Instruções para o hands-on**
 - Básico de instalação do ns-3 indo até o “hello simulator”;
 - Listagem de problemas/soluções já enfrentados;
 - Boas práticas.

 - **Equipe de execução:** Todo o time contribuirá para o desenvolvimento deste hands-on. **Prazo:** 17/09/2019.
 
 
 - **Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br)- **Prazo:** 24/09/2019.
 
### [Hands-on 2: Criação e uso de *patch* para distribuição de código modificado](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/master/ferramentas_complementares/HD_02/fc_hd02.ipynb)

**Objetivos**
 - Criação e uso da ferramenta de alteração/atualização *patch*.
 
**Instruções para o hands-on**
 - A definir

 - **Equipe de execução:** A definir.
 
  - **Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br)- **Prazo:** -/-/-.
