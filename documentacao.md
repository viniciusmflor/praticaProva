# praticaProva
parte prática da prova mod. 4

# Documentação

Este documento lista e descreve os estados do sistema implementado no semáforo inteligente.

## 1. Modo Noturno
- **Descrição:** Quando o ambiente está escuro (valor do LDR abaixo do limiar definido), o sistema entra no modo noturno.
  - O LED amarelo deve piscar a cada 1 segundo.
  - Os LEDs vermelho e verde permanecem desligados.
- **Condição de ativação:** O valor do LDR é menor que o limiar definido (`LDR_THRESHOLD`).
- **Comportamento esperado no Serial Monitor:** 
  - Exibe a mensagem `"Modo noturno ativado"`.
  - Exibe os valores do LDR capturados.

## 2. Modo Diurno
- **Descrição:** Quando o ambiente está claro (valor do LDR acima do limiar definido), o sistema entra no modo diurno.
  - O semáforo alterna entre os LEDs verde, amarelo e vermelho na seguinte sequência:
    - **Verde:** 3 segundos.
    - **Amarelo:** 2 segundos.
    - **Vermelho:** 5 segundos.
- **Condição de ativação:** O valor do LDR é maior que o limiar definido (`LDR_THRESHOLD`).
- **Comportamento esperado no Serial Monitor:**
  - Exibe a mensagem `"Modo diurno ativado"`.
  - Exibe os valores do LDR capturados.
  - Mostra as transições entre os estados do semáforo.

## 3. Pressionamento Único do Botão no Vermelho
- **Descrição:** Quando o semáforo está no estado vermelho e o botão é pressionado:
  - O semáforo muda para verde após 1 segundo.
- **Condição de ativação:** 
  - O semáforo está no estado vermelho.
  - O botão é pressionado uma única vez.
- **Comportamento esperado no Serial Monitor:**
  - Exibe a mensagem `"Botão pressionado!"`.
  - Exibe a mensagem `"Mudando para o estado verde"` após 1 segundo.

## 4. Pressionamento Triplo do Botão no Vermelho
- **Descrição:** Quando o semáforo está no estado vermelho e o botão é pressionado três vezes consecutivas:
  - Uma requisição HTTP é enviada.
  - O monitor serial exibe o código de resposta da requisição.
- **Condição de ativação:** 
  - O semáforo está no estado vermelho.
  - O botão é pressionado três vezes em sequência.
- **Comportamento esperado no Serial Monitor:**
  - Exibe a mensagem `"Requisição HTTP enviada"`.
  - Exibe o código de resposta HTTP (como `200`).



![Uploading Captura de Tela 2024-12-13 às 10.42.27.png…]()

![Uploading Captura de Tela 2024-12-13 às 10.42.12.png…]()


