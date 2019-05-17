# mitcard

自作キーボードの習作。

## 特徴

* [Teency LC](https://www.pjrc.com/teensy/teensyLC.html)/[Teency 3.2](https://www.pjrc.com/teensy/teensy31.html)両対応
* 2x2のキーマトリックス
* [スイッチ用PCBソケット](https://yushakobo.jp/shop/a01ps/)使用
* 2つのタッチキー
* [modulo](http://biacco42.hatenablog.com/entry/2018/12/13/063814) 互換のジャック (※1)
* [SparkFun TeencyView](https://www.sparkfun.com/products/14048) 搭載可能 (※2)
* その他I/Oピンを外出し (※2)

※1 ソフトウェアは未対応、また、Teency LCは5Vトレラントではないことに注意。
※2 一部ピンが重複しています。

## 基板以外の材料

### 必須

* [Teency LC](https://www.pjrc.com/teensy/teensyLC.html)、または、[Teency 3.2](https://www.pjrc.com/teensy/teensy31.html)
  どちらかを用意してください。Modulo Pendant 互換を目指す場合は[Teency 3.2](https://www.pjrc.com/teensy/teensy31.html)一択です。
* ピンヘッダ・ピンソケット
  ボード内側のピンは使用していません。ですので、ボードの縁にあるスルーホールの数に対応したピンヘッダ・ピンソケットを用意してください。
  [スイッチサイエンス取扱のSparkFun製のもの](https://www.switch-science.com/catalog/2851/)とピンソケットでもよいです。

## メカニカルキーボードにするのに必須

以下のものをそれぞれ4つ用意してください。

* [スイッチ用PCBソケット](https://yushakobo.jp/shop/a01ps/)使用
* キースイッチ
  MX互換とKailh Chocに対応します。お好きなものをどうぞ。

## Modulo Pendant 互換品にするのに必須

以下のものをそれぞれ2つ用意してください。

* [MJ-4PP-9](http://akizukidenshi.com/catalog/g/gC-06070/)
* 抵抗
  (抵抗の大きさについては現在検討中。)

## OLED をつける場合

[SparkFun TeencyView](https://www.sparkfun.com/products/14048)が使用可能です。

ただし、D/Cを5ピンから21ピンに変更する必要があります。[ここ](https://learn.sparkfun.com/tutorials/teensyview-hookup-guide)を参考にデフォルトのラインを切って、21ピンの方にハンダでつなげてください。

(以降、更新予定)
