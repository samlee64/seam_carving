module Page.Index exposing (..)

import Bootstrap.Card as Card
import Bootstrap.Card.Block as CardBlock
import Bootstrap.Utilities.Flex as Flex
import Bootstrap.Utilities.Spacing as Spacing
import Flags exposing (Flags)
import Html exposing (..)
import Html.Attributes exposing (..)


type Msg
    = NoOp


type alias Model =
    { flags : Flags }


init : Flags -> ( Model, Cmd Msg )
init flags =
    ( { flags = flags }, Cmd.none )


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        NoOp ->
            ( model, Cmd.none )


subscriptions : Model -> Sub Msg
subscriptions _ =
    Sub.none


view : Model -> Html Msg
view model =
    let
        imgSrc =
            model.flags.bucket ++ "/defaults/couple.png"
    in
    div [ Flex.block ]
        [ Card.config [ Card.attrs [ Spacing.mr3 ] ]
            |> Card.header [] [ text "Platform" ]
            |> Card.block [] [ CardBlock.text [] [ text "Platform built run c++ image processing code." ] ]
            |> Card.view
        , a [ style "cursor" "pointer", href "/sc" ]
            [ Card.config [ Card.attrs [] ]
                |> Card.header [] [ text "Seam Carving" ]
                |> Card.block []
                    [ CardBlock.custom <|
                        img [ src imgSrc ] []
                    ]
                |> Card.footer [] []
                |> Card.view
            ]
        ]
