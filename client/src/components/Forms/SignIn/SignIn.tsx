import React, { FormEvent, HTMLProps, useCallback, useState } from "react";
import { useTranslation } from "react-i18next";
import api from "../../../generated/.api_doc.json";

const formStyle: React.CSSProperties = { marginTop: "1.5em" };
const fieldsetStyle: React.CSSProperties = {
  display: "flex",
  flexDirection: "column",
  gap: "0.5em",
  borderRadius: "0.25em"
};

const FIELDS = {
  login: api.paths["/user/sign-in"].post.parameters[0],
  password: api.paths["/user/sign-in"].post.parameters[1]
};

type Props = {
  fieldsetProperties?: HTMLProps<HTMLFieldSetElement>;
  formProperties?: HTMLProps<HTMLFormElement>;
  id: string;
  title?: string;
};

/**
 * onSubmit function
 * @param event the on submit form event
 */
const onSubmitRequest = async (
  event: FormEvent<HTMLFormElement>,
  pendingUpdater: (state: boolean) => void,
  resultUpdater: (success: boolean, error?: string) => void
): Promise<void> => {
  event.preventDefault();
  const fields = [FIELDS.login?.name, FIELDS.password?.name];
  const formElements = event.currentTarget.elements;

  const formData = new Map<string, string>(
    fields.map((fieldName: string | undefined) => {
      if (fieldName === undefined) {
        throw Error("Documentation Error");
      }
      const fieldInput = formElements.namedItem(fieldName);
      if (!fieldInput || !(fieldInput instanceof HTMLInputElement)) {
        throw Error("Invalid form usage");
      }
      return [fieldName, fieldInput.value];
    })
  );

  console.info("start spinner and lock button");
  pendingUpdater(true);
  try {
    const response = await fetch(
      event.currentTarget.getAttribute("action") || "/",
      {
        method: event.currentTarget.getAttribute("method") || "POST",
        headers: {
          "Content-Type": "application/json"
        },
        body: JSON.stringify(Object.fromEntries(formData))
      }
    );

    if (response.status <= 400) {
      console.info("display success notification");
      resultUpdater(true);
    } else {
      console.error("display error notification with code", response.status);
      resultUpdater(false, `Error notification with code ${response.status}`);
    }
  } catch (error) {
    console.error("display error notification", error);
    const message = error instanceof Error ? error.message : "unknown error";
    resultUpdater(false, `Error notification ${message}`);
  }
  console.info("end spinner and enable button");
  pendingUpdater(false);
};

function SignIn({ fieldsetProperties, formProperties, id, title }: Props) {
  const { t } = useTranslation();
  const [pendingState, setPendingState] = useState(false);
  const [errors, setErrors] = useState<Array<string>>([]);
  const onSubmit = useCallback(
    (event) => {
      onSubmitRequest(
        event,
        setPendingState,
        (success: boolean, error?: string) => {
          if (success) {
            console.log("success");
          }
          if (error) {
            setErrors([...errors, error]);
          }
        }
      );
    },
    [errors]
  );

  return (
    <>
      <form
        style={formStyle}
        method="POST"
        action={`/api/${api.paths["/user/sign-in"]}`}
        id={id}
        onSubmit={onSubmit}
        {...formProperties}
      >
        <fieldset style={fieldsetStyle} {...fieldsetProperties}>
          <legend>{t(title || "FORMS.SIGN_IN.TITLE")}</legend>
          <label htmlFor="FORMS.SIGN_IN.FIELDS.IDENTIFIER">
            {t("FORMS.SIGN_IN.FIELDS.IDENTIFIER.LABEL")}
          </label>
          <input
            type="text"
            name={FIELDS.login?.name}
            id="FORMS.SIGN_IN.FIELDS.IDENTIFIER"
            required={FIELDS.login?.required}
          />
          <label htmlFor="FORMS.SIGN_IN.FIELDS.IDENTIFIER">
            {t("FORMS.SIGN_IN.FIELDS.PASSWORD.LABEL")}
          </label>
          <input
            type="password"
            name={FIELDS.password?.name}
            id="FORMS.SIGN_IN.FIELDS.PASSWORD"
            required={FIELDS.password?.required}
          />
          <input
            disabled={pendingState}
            type="submit"
            value={String(t("FORMS.SIGN_IN.FIELDS.SUBMIT.LABEL"))}
          />
        </fieldset>
      </form>
      {pendingState ? <div>loading</div> : null}
    </>
  );
}

export default SignIn;
