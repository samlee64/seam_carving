module Request.Request exposing (..)

import Bytes.Decode as Bytes
import Flags exposing (Flags)
import Http
import HttpBuilder as HB
import Json.Decode as Decode exposing (Decoder, string)
import RemoteData as RD exposing (WebData)
import Url.Builder as UrlBuilder


get : Flags -> List String -> HB.RequestBuilder ()
get flags path =
    UrlBuilder.crossOrigin flags.api path []
        |> HB.get


withExpect : Decoder a -> (WebData a -> msg) -> HB.RequestBuilder () -> HB.RequestBuilder msg
withExpect decoder msg =
    HB.withExpect (Http.expectJson (RD.fromResult >> msg) decoder)


withExpectBytes : Bytes.Decoder a -> (WebData a -> msg) -> HB.RequestBuilder () -> HB.RequestBuilder msg
withExpectBytes decoder msg =
    HB.withExpect (Http.expectBytes (RD.fromResult >> msg) decoder)


request : HB.RequestBuilder msg -> Cmd msg
request =
    HB.request


healthCheck : Flags -> (WebData String -> msg) -> Cmd msg
healthCheck flags msg =
    let
        logs =
            Debug.log "flags.api" flags.api
    in
    UrlBuilder.crossOrigin flags.api [ "health" ] []
        |> HB.get
        |> HB.withExpect (Http.expectJson (RD.fromResult >> msg) string)
        |> HB.request



{-
   withExpect :
           |> HB.withExpect (Http.expectJson (RD.fromResult >> msg) decoder
           |> HB.request
-}
