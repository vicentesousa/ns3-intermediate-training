# Campanhas de investigação de desempenho do Wi-Fi

## Fase II: Campanhas de investigação com análise de desempenho do Wi-Fi

### [Hands-on 01: Campanha 0](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_02/HD_01/f02_hd01.ipynb)
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

**[Wi-Fi 802.11a](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_02/HD_01/f02_hd01_wifia.ipynb)** - **Equipe de execução:** Malco Dantas (malco@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wi-Fi 802.11b](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_02/HD_01/f02_hd01_wifib.ipynb)**  - **Equipe de execução:** Ravan Oliveira (ravan@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wi-Fi 802.11g](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_02/HD_01/f02_hd01_wifig.ipynb)** - **Equipe de execução:** Lucas Ismael (lucasismael.gppcom@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wi-Fi 802.11n](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_02/HD_01/f02_hd01_wifin.ipynb)**  - **Equipe de execução:** Lucas Ismael (lucasismael.gppcom@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wi-Fi 802.11ac](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_02/HD_01/f02_hd01_wifiac.ipynb)**  - **Equipe de execução:** Jéssika Cristina (jessie@ufrn.edu.br) - **Prazo:** 04/07/2019;

**[Wi-Fi 802.11ax](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_02/HD_01/f02_hd01_wifiax.ipynb)**  - **Equipe de execução:** Daniel Flor (danielflor@ufrn.edu.br) - **Prazo:** 04/07/2019;

**Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) e Prof. Vicente Sousa (vicente.sousa@ufrn.edu.br) - **Última Atualização:** 07/2019.


### [Hands-on 02: Campanha 1](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_02/HD_02/f02_hd02.ipynb)
**Objetivos**
 - Analisar o comportamento do sistema com o aumento do **tráfego oferecido**;
 - Analisar o comportamento do sistema com o aumento da **duração da simulação**;
 - Analisar o comportamento do sistema com o aumento do **número de Jobs da simulação**.

**Instruções para o hands-on**
 - Utilizar os scripts de criação das campanhas das Ferramentas Complementares Hands-on 1;
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

 - **Equipe de execução:** Daniel Flor (danielflor@ufrn.edu.br) e Daniel Luna (danielro@ufrn.edu.br).

- **Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) - **Última Atualização:** --/--.

### [Hands-on 03: Campanha 2](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_02/HD_03/f02_hd03.ipynb)

**Objetivos**
 - Analisar o comportamento de diferentes algoritmos de adaptação de taxa com o aumento do número de STAs (tráfego oferecido constante).

**Instruções para o hands-on**
 - Utilizar os scripts de criação das campanhas das Ferramentas Complementares Hands-on 1;
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

 - **Equipe de execução:** Lucas Ismael (lucasismael.gppcom@ufrn.edu.br) e Ravan Oliveira (ravan@ufrn.edu.br).
 

- **Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) - **Última Atualização:** 10/2019.

## Fase III: Avaliação do DCF e EDCA no Wi-Fi

### [Hands-on 01: Campanha 1](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_03/HD_01/f03_hd01.ipynb)
**Objetivos**
 - Avaliar o comportamento do terminal escondido no ns-3.

**Instruções para o hands-on**
 - Utilizar os scripts de criação das campanhas das Ferramentas Complementares Hands-on 1;
 - Fazer simulações variando:
 	- Algoritmos de adaptação de taxa: MinstrelHtWifiManager e ConstantRateWifiManager;
 	- Camada PHY: 802.11ac;
 	- Grid: 20m x 20m; 
 	- Número de STAs: variando de 1 a 10;
 	- Simulation runs: 1000 Jobs;
 	- STAs parados e posicionados aleatoriamente a cada Job (AP no centro);
 	- Taxa oferecida: valor máximo da camada PHY escolhida;
	- Tempo de simulação: 200 segundos;
 - Estudo 1 (semelhante a Fase 02: campanha 0): 
 	- Comparação de Throughput vs distância dos algoritmos de adaptação de taxa;
 	- Variar distância de de 10 a 200 m de 5 em 5 metros (tentar mostrar bem o gráfico em escada do controle de taxa);
 - Estudo 2: 
 	- Comparação de Throughput vs número de STAs com o usando os algoritmos de adaptação de taxa;
 	- STAs distribuídas em um raio igual a 15 metros, com taxa de transmissão de 1.5Mbps;
 - Análise deve ser feita para **Downlink** e **Uplink**.

 - **Equipe de execução:** Ravan Oliveira (ravan@ufrn.edu.br).

- **Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) - **Última Atualização:** --/--.


### [Hands-on 02: Campanha 2](https://nbviewer.jupyter.org/github/vicentesousa/ns3-intermediate-training/blob/icTraining/Wi-Fi/fase_03/HD_02/f03_hd02.ipynb)
**Objetivos**
 - Comparar DCF com EDCA em uma configuração com múltiplos serviços en cada STA tenha quatro streams de tráfego com diferentes QoS tags: AC_VO, AC_VI, AC_BE e AC_BK (WifiQosTag).

**Instruções para o hands-on**
 - Utilizar os scripts de criação das campanhas das Ferramentas Complementares Hands-on 1;
 - Fazer simulações variando:
 	- Algoritmos de adaptaçãode taxa: MinstrelHtWifiManager;
 	- Camada PHY: 802.11ac (5GHz);
 	- Cada streams consistindo de pacotes de 1500 bytes de payload com 100 kbps de tráfego CBR;
 	- Grid: 20m x 20m; 
 	- Número de STAs: variando de 1 a 30;
 	- Simulation runs: 100 Jobs (mínimo);
 	- STAs parados e posicionados aleatoriamente a cada Job (AP no centro);
 	- Taxa oferecida: valor máximo da camada PHY escolhida;
 	- Tempo de simulação: 200 segundos;
 - Estudo 1: 
 	- Throughput separado por serviço com 95% de intervalo de confiança;
 	- Carga de usuários: variando de 1 a 30;
 - Estudo 2: 
 	- PLR separado por serviço com 95% de intervalo de confiança;
 	- Carga de usuários: variando de 1 a 30;
 - Estudo 3: 
 	- Atrasos separado por serviço com 95% de intervalo de confiança;
 	- Carga de usuários: variando de 1 a 30.
 - Repetir estudos do TCC do Rodrigo;
 - Análise deve ser feita para **Downlink** e **Uplink**.

 - **Equipe de execução:** Lucas Ismael (lucasismael.gppcom@ufrn.edu.br).

- **Equipe de revisão:** Daniel Luna (danielro@ufrn.edu.br) - **Última Atualização:** 03/2020.