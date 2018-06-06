type market = {
  id: int,
  name: string,
  symbol: string,
  rank: int,
  price_usd: float,
  price_eur: float,
  percent_change_24h: float,
};

type marketData = {
  markets: array(market),
  totalSize: int,
};

let apiUrl = (limit, offset) =>
  "https://api.coinmarketcap.com/v2/ticker/?limit="
  ++ string_of_int(limit)
  ++ "&start="
  ++ string_of_int(offset)
  ++ "&structure=array&convert=EUR";

module Decode = {
  let floatDecoder = Json.Decode.float; /* shadow warning? */
  let parseMarketJson = json : market =>
    Json.Decode.{
      id: json |> field("id", int),
      name: json |> field("name", string),
      symbol: json |> field("symbol", string),
      rank: json |> field("rank", int),
      price_usd: json |> at(["quotes", "USD", "price"], floatDecoder),
      price_eur: json |> at(["quotes", "EUR", "price"], floatDecoder),
      percent_change_24h:
        json |> at(["quotes", "USD", "percent_change_24h"], floatDecoder),
    };

  let parseMarketResponseJson = json : marketData => {
    markets:
      json |> Json.Decode.field("data", parseMarketJson |> Json.Decode.array),
    totalSize:
      json
      |> Json.Decode.at(
           ["metadata", "num_cryptocurrencies"],
           Json.Decode.int,
         ),
  };
};

let fetchMarkets = (limit, offset) =>
  Js.Promise.(
    Axios.get(apiUrl(limit, offset))
    |> then_(response =>
         switch (Decode.parseMarketResponseJson(response##data)) {
         | exception e =>
           Js.log2("Decoding error", e);
           reject(e);
         | marketData => resolve(marketData)
         }
       )
  );
