module Page.SeamCarving.Subscriptions exposing (subscriptions)

import Bootstrap.Dropdown as Dropdown
import Page.SeamCarving.Model exposing (Model)
import Page.SeamCarving.Msg exposing (GrowFormMsg(..), Msg(..))


subscriptions : Model -> Sub Msg
subscriptions model =
    Sub.batch
        [ Dropdown.subscriptions model.growForm.numStepsDropdown (\s -> GrowFormMsg <| NumStepsDropdown s)
        ]
