let component = ReasonReact.statelessComponent("Spinner");

module Styles = {
  open Css;

  let stretchdelay =
    keyframes([
      (0, [transform(scaleY(0.4))]),
      (20, [transform(scaleY(1.0))]),
      (40, [transform(scaleY(0.4))]),
      (100, [transform(scaleY(0.4))]),
    ]);

  let spinner =
    style([
      margin2(~h=auto, ~v=px(100)),
      width(px(50)),
      height(px(40)),
      textAlign(center),
      fontSize(px(10)),
      selector(
        " div",
        [
          backgroundColor(hex("000")),
          height(pct(100.0)),
          width(px(6)),
          display(inlineBlock),
          marginLeft(px(3)),
          animation(
            ~duration=1200,
            ~iterationCount=infinite,
            ~timingFunction=easeInOut,
            stretchdelay,
          ),
        ],
      ),
      selector(" .rect2", [animationDelay(-1100)]),
      selector(" .rect3", [animationDelay(-1000)]),
      selector(" .rect4", [animationDelay(-900)]),
      selector(" .rect5", [animationDelay(-800)]),
    ]);
};

let make = _children => {
  ...component,
  render: _self =>
    <div className=Styles.spinner>
      <div className="rect1" />
      <div className="rect2" />
      <div className="rect3" />
      <div className="rect4" />
      <div className="rect5" />
    </div>,
};
