type request('a) =
  | Loading
  | Some('a)
  | Error;

type state = {
  markets: request(array(MarketData.market)),
  totalSize: int,
  rowsPerPage: int,
  currentPage: int,
};

type action =
  | MarketsLoading
  | MarketsLoaded(MarketData.marketData)
  | MarketsError
  | FetchPage(int);

module Styles = {
  open Css;

  let container =
    style([
      maxWidth(px(1024)),
      margin2(~h=auto, ~v=px(0)),
      fontFamily("Helvetica Neue"),
    ]);

  let title =
    style([
      fontWeight(200),
      fontSize(em(2.0)),
      fontFamily("Helvetica Neue"),
      marginBottom(em(2.0)),
      textAlign(center),
    ]);

  let table =
    style([
      width(pct(100.0)),
      borderColor(hex("333")),
      borderStyle(solid),
      borderWidth(px(1)),
      borderCollapse(`collapse),
      selector(" th", [padding2(~h=px(0), ~v=px(10))]),
      selector(
        " tr",
        [
          borderColor(hex("333")),
          borderStyle(solid),
          borderWidth(px(1)),
        ],
      ),
    ]);
};

let s = ReasonReact.string;
let eventTarget = event => ReactDOMRe.domElementToObj(
                             ReactEventRe.Form.target(event),
                           )##value;

let limit = 15;

let component = ReasonReact.reducerComponent("App");

let fetchMarkets = (self, ~limit, ~offset) => {
  self.ReasonReact.send(MarketsLoading);
  MarketData.fetchMarkets(limit, offset)
  |> Js.Promise.then_(marketData => {
       self.ReasonReact.send(MarketsLoaded(marketData));
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(_err => {
       self.ReasonReact.send(MarketsError);
       Js.Promise.resolve();
     })
  |> ignore;
};

let make = _children => {
  ...component,
  initialState: () => {
    markets: Loading,
    totalSize: 0,
    rowsPerPage: limit,
    currentPage: 0,
  },
  didMount: self =>
    fetchMarkets(self, ~limit, ~offset=self.state.currentPage),
  reducer: (action, state) =>
    switch (action) {
    | MarketsLoading => ReasonReact.Update({...state, markets: Loading})
    | MarketsLoaded({markets, totalSize}) =>
      ReasonReact.Update({...state, markets: Some(markets), totalSize})
    | MarketsError => ReasonReact.Update({...state, markets: Error})
    | FetchPage(offset) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, currentPage: offset},
        fetchMarkets(~limit, ~offset=offset * limit),
      )
    },
  render: self => {
    let marketItem = (market: MarketData.market) =>
      <MarketItemRow key=(string_of_int(market.id)) market />;
    let marketsComponent =
      switch (self.state.markets) {
      | Some(markets) =>
        <MaterialUI.Table>
          <MaterialUI.TableHead>
            <MaterialUI.TableRow>
              <MaterialUI.TableCell>
                (s("Rank"))
                <MaterialUI.TableSortLabel />
              </MaterialUI.TableCell>
              <MaterialUI.TableCell>
                (s("Name (Symbol)"))
              </MaterialUI.TableCell>
              <MaterialUI.TableCell>
                (s("Price (USD)"))
              </MaterialUI.TableCell>
              <MaterialUI.TableCell>
                (s("Price (EUR)"))
              </MaterialUI.TableCell>
              <MaterialUI.TableCell>
                (s("Change (24h)"))
              </MaterialUI.TableCell>
            </MaterialUI.TableRow>
          </MaterialUI.TableHead>
          <MaterialUI.TableBody>
            (markets |> Array.map(marketItem) |> ReasonReact.array)
          </MaterialUI.TableBody>
          <MaterialUI.TableFooter>
            <MaterialUI.TableRow>
              <MaterialUI.TablePagination
                count=(`Int(self.state.totalSize))
                page=(`Int(self.state.currentPage))
                rowsPerPage=(`Int(self.state.rowsPerPage))
                rowsPerPageOptions=[|limit|]
                onChangePage=((_e, offset) => self.send(FetchPage(offset)))
              />
            </MaterialUI.TableRow>
          </MaterialUI.TableFooter>
        </MaterialUI.Table>
      | Error => ReasonReact.string("Error loading market data.")
      | Loading => <Spinner />
      };

    <div className=Styles.container>
      <header>
        <h1 className=Styles.title>
          (s("Top Cryptocurrencies by Market Capitalization"))
        </h1>
      </header>
      marketsComponent
    </div>;
  },
};
