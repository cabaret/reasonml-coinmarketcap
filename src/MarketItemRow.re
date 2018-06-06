module Styles = {
  open Css;
};

let s = ReasonReact.string;

let component = ReasonReact.statelessComponent("MarketItemRow");

let make = (~market: MarketData.market, _children) => {
  ...component,
  render: _self =>
    <MaterialUI.TableRow>
      <MaterialUI.TableCell>
        (s(string_of_int(market.rank)))
      </MaterialUI.TableCell>
      <MaterialUI.TableCell>
        <strong> (s(market.name ++ " (" ++ market.symbol ++ ")")) </strong>
      </MaterialUI.TableCell>
      <MaterialUI.TableCell numeric=true>
        (s("$ " ++ string_of_float(market.price_usd)))
      </MaterialUI.TableCell>
      <MaterialUI.TableCell numeric=true>
        (s({js|€|js} ++ " " ++ string_of_float(market.price_eur)))
      </MaterialUI.TableCell>
      <MaterialUI.TableCell numeric=true>
        (s(string_of_float(market.percent_change_24h) ++ "%"))
      </MaterialUI.TableCell>
    </MaterialUI.TableRow>,
};
