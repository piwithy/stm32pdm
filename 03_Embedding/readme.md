<h1> 03_Embedding</h1>

Ce dossier contient les sources des 3 démonstrateurs développées pour la cible STM32 (carte STM32F429I-DISC1). Ces démonstrateurs sont les suivants :
- [parrot](parrot/readme.md ): Application « Perroquet ». Lors de l’appui sur le bouton `USER` la carte commence l’enregistrement du son ambiant, lors d’un deuxième appui la carte rejoue jusqu’aux 3 dernières secondes sur le DAC puis elle transmet les données WAV via le port série.
- [Direct Output](direct_output/readme.md ): ce démonstrateur joue en temps réel sur le DAC les sons captés par la carte
- [Digital Recorder](digital_recoder/readme.md) : Ce démonstrateur lors de l’appui sur le bouton `USER` le démonstrateur enregistre les sons ambiants sur une clé USB lors d’un deuxième appui, il arrête l’enregistrement.

## Peripheriques utilisés Par les demonstrateur

|                   |GPIO   | DAC   | RCC   | SAI 1 | SYS   | USART 1   | TIM 2 | USB_HS |
|:---               |:---:  |:---:  |:---:  |:-----:|:---:  |:-------:  |:-----:|:------:|
|Parrot             |X      |X      |X      |X      |X      |X          |X      |        |
|Direct Output      |X      |X      |X      |X      |X      |           |X      |        |
|Digital Recoder    |X      |       |X      |X      |X      |X          |       |X       |

## Configuration des Peripheriques

### Pinout

<table>
	<thead>
		<tr>
			<th>PIN</th>
			<th>Signal</th>
            <th>Commentaire</th>
            <th>Péripherique</th>
		<tr>
	</thead>
	<tbody>
		<tr>
			<td>PA0</td>
			<td>USER_BUTTON</td>
			<td>Bouton Bleu de la carte</td>
			 <td style="text-align: center; vertical-align: middle;" rowspan="3">GPIO</td>
        </tr>
        <tr>
			<td>PG13</td>
			<td>LD3</td>
			<td>LED Verte</td>
        </tr>
		<tr>
			<td>PG14</td>
			<td>LD4</td>
			<td>LED Rouge</td>
		<tr>
        <tr>
        	<td>PA4</td>
            <td>DAC_OUT1</td>
            <td>Canal Gauche du DAC (Fil Rouge)</td>
            <td style="text-align: center; vertical-align: middle;" rowspan="2">DAC</td>
        </tr>
        <tr>
        	<td>PA5</td>
            <td>DAC_OUT2</td>
            <td>Canal Droit du DAC (Fil Blanc)</td>
        </tr>
        <tr>
        	<td>PH0</td>
            <td>RCC_OSC_IN</td>
            <td style="vertical-align: middle;" rowspan="2">Signaux de l'oscillateur</td>
            <td style="text-align: center; vertical-align: middle;" rowspan="2">RCC</td>
        </tr>
         <tr>
        	<td>PH1</td>
            <td>RCC_OSC_OUT</td>
        </tr>
        <tr>
        	<td>PE4</td>
            <td>SAI_FS_A</td>
            <td>SAI SELECT</td>
            <td style="text-align: center; vertical-align: middle;" rowspan="3">SAI 1</td>
        </tr>
         <tr>
        	<td>PE5</td>
            <td>SAI_SCK_A</td>
            <td>SAI CLOCK</td>
        </tr>
        <tr>
        	<td>PE6</td>
            <td>SAI_SD_A</td>
            <td>SAI DATA</td>
        </tr>
        <tr>
        	<td>PA13</td>
            <td>SYS_JTMS-SWDIO</td>
            <td style="vertical-align: middle;" rowspan="2">Lignes de DEBUG</td>
            <td style="text-align: center; vertical-align: middle;" rowspan="2">SYS</td>
        </tr>
         <tr>
        	<td>PA14</td>
            <td>SYS_JTCK-SWCLK</td>
        </tr>
        <tr>
        	<td>PA9</td>
            <td>USART1_TX</td>
            <td>Canal TX de l'USART 1</td>
             <td style="text-align: center; vertical-align: middle;" rowspan="2">USART 1</td>
        </tr>
        <tr>
        	<td>PA9</td>
            <td>USART1_RX</td>
            <td>Canal RX de l'USART 1</td>
        </tr>
        <tr>
            <td> PB13 </td>
            <td> USB_OTG_HS_VBUS </td>
            <td> Signal USB 2.0 VBUS</td>
            <td style="text-align: center; vertical-align: middle;" rowspan="4"> USB_HS </td>
        </tr>
        <tr>
            <td> PB14 </td>
            <td> USB_OTG_HS_DM</td>
            <td> Signal USB 2.0 D-</td>
        </tr>
        <tr>
            <td> PD15</td>
            <td> USB_OTG_HS_DP</td>
            <td> Signal USB 2.0 D+ </td>
        </tr><tr>
            <td> PC4 </td>
            <td> USB_PSO </td>
            <td> Active l'alimentation de l'esclave USB par la carte (0 &rarr; Alimenté, 1 &rarr; Non alimenté)</td>
        </tr>
	</tbody>
</table>
